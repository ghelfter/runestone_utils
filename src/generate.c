/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                   generate.c
 **************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generate.h"
#include "gen_function.h"
#include "functions.h"
#include "util.h"

#define NQUESTIONS 3
#define MAX_BUFFER_SIZE 128
#define MAX_BIG_BUFFER_SIZE 4096

/**************************************************
 *               Static definitions
 **************************************************/

/* Function pointer table */
static question_func_t question_function_table[NQUESTIONS] = {
    NULL,
    generate_multiple_choice,
    generate_fill_in_blanks
};

/* Static and global variables */
static const char* question_type_table[NQUESTIONS] = {
    "None",
    "mchoice",
    "fillintheblank"
};

static const char *question_type_desc[NQUESTIONS] = {
    "None",
    "Multiple Choice",
    "Fill in the Blank"
};

/**************************************************
 *               External functions
 **************************************************/
int generate_question_type(FILE *fd, int question_type, const char *title)
{
    int retcode = GENERATE_SUCCESS;

    if(fd == NULL || title == NULL)
    {
        retcode = GENERATE_NULL_PTR;
    }
    else if(question_type < 1 || question_type > (NQUESTIONS-1))
    {
        retcode = GENERATE_BAD_CODE;
    }
    else
    {
        fprintf(fd, ".. %s:: %s\n", question_type_table[question_type], title);
    }

    return retcode;
}

void print_question_descriptions(FILE *fd)
{
    unsigned int i = 1u;
    fprintf(stdout, "The following question types are supported:\n");

    for(i = 1u; i < NQUESTIONS; ++i)
    {
        fprintf(fd, "(%u) %s\n", i, question_type_desc[i]);
    }
}

int acquire_question_type(FILE *fd, int *in_qtype)
{
    int retcode = GENERATE_SUCCESS;
    char read_buff[MAX_BUFFER_SIZE];
    char *endptr = NULL;
    int question_type = 0;

    if(fd == NULL || in_qtype == NULL)
    {
        retcode = GENERATE_NULL_PTR;
    }
    else
    {
        fprintf(stdout, "Please enter question number: ");
        fgets(read_buff, MAX_BUFFER_SIZE, fd);

        generate_chomp(read_buff, MAX_BUFFER_SIZE, '\n');

        /* Parse number out of the string buffer */
        question_type = strtol(read_buff, &endptr, 0);

        /* Question number failed to parse */
        if(*endptr != '\0' || endptr == read_buff)
        {
            retcode = GENERATE_PARSE_ERROR;
        }
        else
        {
            *in_qtype = question_type;
        }
    }

    return retcode;
}

/* Function that will call the appropriate function */
int generate_question(FILE *fd, int question_type, const char *title)
{
    int retcode = GENERATE_SUCCESS;
    if(fd == NULL || title == NULL)
    {
        retcode = GENERATE_NULL_PTR;
    }
    else
    {
        /* Assert question type is a valid number */
        if(question_type < 1 || question_type > (NQUESTIONS-1))
        {
            retcode = GENERATE_BAD_CODE;
        }
        else
        {
            retcode = question_function_table[question_type](fd, stdin, title);
        }
    }

    return retcode;
}

int acquire_question_id(FILE *fd, char *buffer, int buffer_size)
{
    int retcode = GENERATE_SUCCESS;

    if(fd == NULL || buffer == NULL)
    {
        retcode = GENERATE_NULL_PTR;
    }
    else
    {
        fprintf(stdout, "Please enter unique database ID: ");
        fgets(buffer, buffer_size, fd);

        /* Chomp the string */
        generate_chomp(buffer, buffer_size, '\n');
    }

    return retcode;
}
