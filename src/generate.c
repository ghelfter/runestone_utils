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

#define NQUESTIONS 3
#define MAX_BUFFER_SIZE 128
#define MAX_BIG_BUFFER_SIZE 4096

#define MAX(x,y) ((x) > (y) ? (x) : (y))

/**************************************************
 *             Static function headers
 **************************************************/
static void generate_chomp(char *buffer, int buffer_len, char delim);

/* Functions for reading specific question types */
static int generate_multiple_choice(FILE *fd, FILE *infile, const char * file);
static int generate_fill_in_blanks(FILE *fd, FILE *infile, const char * file);
static void prompt_and_chomp(const char *prompt, char *buffer, int size, 
                             FILE *infile);

/* Function pointer table */
typedef int (*question_func_t)(FILE *fd, FILE *infile, const char * file);
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

/**************************************************
 *             Static module functions
 **************************************************/
static void generate_chomp(char *buffer, int buffer_len, char delim)
{
    int len = strnlen(buffer, buffer_len);
    int m_off = (len-1) < 0 ? 0 : (len-1);

    if(*(buffer+m_off) == delim)
    {
        *(buffer+m_off) = '\0';
    }
}

static int generate_multiple_choice(FILE *fd, FILE *infile, const char *title)
{
    int retcode = GENERATE_SUCCESS;
    char read_buff[MAX_BIG_BUFFER_SIZE];
    char *endptr = NULL;
    int i = 0;
    int tmp = 0;

    /* Set of arrays of strings for the question variables */
    char **q_tags = NULL;
    char **q_texts = NULL;
    char **q_responses = NULL;
    char **q_answers = NULL;
    char *question_text = NULL;

    int q_count = 0; /* Count of the number of choices */
    int a_count = 0; /* Answer count */

    if(fd == NULL || infile == NULL || title == NULL)
    {
        retcode = GENERATE_NULL_PTR;
        goto CLEANUP;
    }

    fprintf(stdout, "Please enter the number of multiple"
                     " choice questions you want: ");
    fgets(read_buff, MAX_BIG_BUFFER_SIZE, infile);
    generate_chomp(read_buff, MAX_BIG_BUFFER_SIZE, '\n');

    /* Parse number out of the string buffer */
    q_count = strtol(read_buff, &endptr, 0);

    /* Question number failed to parse */
    if(*endptr != '\0' || endptr == read_buff)
    {
        retcode = GENERATE_PARSE_ERROR;
        goto CLEANUP;
    }

    /* Check for invalid (negative or zero) question numbers */
    if(q_count < 1)
    {
        retcode = GENERATE_INVALID_NUM;
        goto CLEANUP;
    }

    /* Allocate memories */
    q_tags = malloc(sizeof(char*) * q_count);
    if(q_tags == NULL)
    {
        retcode = GENERATE_BAD_ALLOC;
        goto CLEANUP;
    }
    memset(q_tags, 0x00, sizeof(char*) * q_count);

    q_texts = malloc(sizeof(char*) * q_count);
    if(q_texts == NULL)
    {
        retcode = GENERATE_BAD_ALLOC;
        goto CLEANUP;
    }
    memset(q_texts, 0x00, sizeof(char*) * q_count);

    q_responses = malloc(sizeof(char*) * q_count);
    if(q_responses == NULL)
    {
        retcode = GENERATE_BAD_ALLOC;
        goto CLEANUP;
    }
    memset(q_responses, 0x00, sizeof(char*) * q_count);

    q_answers = malloc(sizeof(char*) * q_count);
    if(q_answers == NULL)
    {
        retcode = GENERATE_BAD_ALLOC;
        goto CLEANUP;
    }
    memset(q_answers, 0x00, sizeof(char*) * q_count);

    /* Acquire question text */
    fprintf(stdout, "Enter the question text:\n");
    fgets(read_buff, MAX_BIG_BUFFER_SIZE, infile);
    generate_chomp(read_buff, MAX_BIG_BUFFER_SIZE, '\n');

    tmp = strnlen(read_buff, MAX_BIG_BUFFER_SIZE);
    question_text = malloc(sizeof(char) * (tmp+1));
    if(question_text == NULL)
    {
        retcode = GENERATE_BAD_ALLOC;
        goto CLEANUP;
    }
    strncpy(question_text, read_buff, tmp+1);

    for(i = 0; i < q_count; ++i)
    {
        fprintf(stdout, "Question #%d:\n", i+1);

        /* Acquire label */
        prompt_and_chomp("Please enter label: ", read_buff,
                         MAX_BIG_BUFFER_SIZE, infile);
        tmp = strnlen(read_buff, MAX_BIG_BUFFER_SIZE);
        fprintf(stdout, "%d\n", tmp);
        *(q_tags+i) = malloc(sizeof(char) * (tmp+1));
        if(*(q_tags+i) == NULL)
        {
            retcode = GENERATE_BAD_ALLOC;
            goto CLEANUP;
        }
        strncpy(*(q_tags+i), read_buff, tmp+1);

        /* Acquire question text */
        prompt_and_chomp("Please enter question text: ", read_buff,
                         MAX_BIG_BUFFER_SIZE, infile);
        tmp = strnlen(read_buff, MAX_BIG_BUFFER_SIZE);
        *(q_texts+i) = malloc(sizeof(char) * (tmp+1));
        if(*(q_texts+i) == NULL)
        {
            retcode = GENERATE_BAD_ALLOC;
            goto CLEANUP;
        }
        strncpy(*(q_texts+i), read_buff, tmp+1);

        /* Acquire question response */
        prompt_and_chomp("Please enter question response: ", read_buff,
                         MAX_BIG_BUFFER_SIZE, infile);
        tmp = strnlen(read_buff, MAX_BIG_BUFFER_SIZE);
        *(q_responses+i) = malloc(sizeof(char) * (tmp+1));
        if(*(q_responses+i) == NULL)
        {
            retcode = GENERATE_BAD_ALLOC;
            goto CLEANUP;
        }
        strncpy(*(q_responses+i), read_buff, tmp+1);
    }
    prompt_and_chomp("Please enter the number of correct answers: ",
                     read_buff, MAX_BIG_BUFFER_SIZE, infile);
    endptr = NULL;
    a_count = strtol(read_buff, &endptr, 0);
    if(*endptr != '\0' || endptr == read_buff)
    {
        retcode = GENERATE_PARSE_ERROR;
        goto CLEANUP;
    }
    for(i = 0; i < a_count; ++i)
    {
        prompt_and_chomp("Please enter a correct answer label: ",
                         read_buff, MAX_BIG_BUFFER_SIZE, infile);
    }

    /* Write the title string to the output file */
    tmp = generate_question_type(fd, QUESTION_TYPE_MULTI_CHOICE, title);
    if(tmp != GENERATE_SUCCESS)
    {
        retcode = tmp;
        goto CLEANUP;
    }

    /* Go through and write the question to the outpout file */
    for(i = 0; i < q_count; ++i)
    {
        fprintf(fd, "    :answer_%s: %s\n", q_tags[i], q_texts[i]);
    }
    for(i = 0; i < q_count; ++i)
    {
        fprintf(fd, "    :feedback_%s: %s\n", q_tags[i], q_responses[i]);
    }

    fprintf(fd, "\n    %s\n", question_text);

CLEANUP:
    /* Clear any memory */
    if(q_tags != NULL)
    {
        for(i = 0; i < q_count; ++i)
        {
            if(q_tags[i] != NULL)
            {
                free(q_tags[i]);
            }
        }
        free(q_tags);
        q_tags = NULL;
    }
    if(q_texts != NULL)
    {
        for(i = 0; i < q_count; ++i)
        {
            if(q_texts[i] != NULL)
            {
                free(q_texts[i]);
            }
        }
        free(q_texts);
        q_texts = NULL;
    }
    if(q_responses != NULL)
    {
        for(i = 0; i < q_count; ++i)
        {
            if(q_responses[i] != NULL)
            {
                free(q_responses[i]);
            }
        }
        free(q_responses);
        q_responses = NULL;
    }
    if(q_answers != NULL)
    {
        for(i = 0; i < q_count; ++i)
        {
            if(q_answers[i] != NULL)
            {
                free(q_answers[i]);
            }
        }
        free(q_answers);
        q_answers = NULL;
    }
    if(question_text != NULL)
    {
        free(question_text);
        question_text = NULL;
    }

    return retcode;
}

static int generate_fill_in_blanks(FILE *fd, FILE *infile, const char *title)
{
    int retcode = GENERATE_SUCCESS;

    if(fd == NULL || infile == NULL || title == NULL)
    {
        retcode = GENERATE_NULL_PTR;
        goto CLEANUP;
    }

CLEANUP:
    return retcode;
}

static void prompt_and_chomp(const char *prompt, char *buffer, int size, 
                             FILE *infile)
{
    fprintf(stdout, "%s", prompt);
    fgets(buffer, size, infile);
    generate_chomp(buffer, size, '\n');
}
