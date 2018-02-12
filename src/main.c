/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                     main.c
 **************************************************/

#define _POSIX_C_SOURCE 200809L

/* C standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generate.h"
#include "gen_function.h"

#define NARGS 2
#define MAIN_BUFFER_SIZE 2048

void print_usage(FILE *fout);

int main(int argc, char **argv)
{
    int retcode = EXIT_SUCCESS;
    int retval = 0;
    int question_code = 0;
    int tmp = 0;
    char buffer[MAIN_BUFFER_SIZE];
    FILE *fd = NULL;

    /* Buffers for storing values */
    char *title = NULL;

    if(argc < NARGS)
    {
        fd = stdout;
    }
    else
    {
        /* Open the file, check for failure */
        fd = fopen(argv[1], "w+");
        if(fd == NULL)
        {
            retcode = EXIT_FAILURE;
            goto CLEANUP;
        }
    }

    print_question_descriptions(stdout);

    /* Acquire question type */
    retval = acquire_question_type(stdin, &question_code);

    if(retval != GENERATE_SUCCESS)
    {
        retcode = EXIT_FAILURE;
        /* Stub to add print statement for the failure */
        fprintf(stderr, "Failed to acquire question number.\n");
        goto CLEANUP;
    }

    /* Acquire title and allocate title buffer */
    retval = acquire_question_id(stdin, buffer, MAIN_BUFFER_SIZE);
    tmp = strnlen(buffer, MAIN_BUFFER_SIZE);
    title = malloc(sizeof(char) * (tmp+1));
    if(title == NULL)
    {
        retcode = EXIT_FAILURE;
        goto CLEANUP;
    }
    strncpy(title, buffer, tmp);
    *(title+tmp) = '\0';

    retval = generate_question(fd, question_code, title);
    if(retval != GENERATE_SUCCESS)
    {
        retcode = EXIT_FAILURE;
        fprintf(stderr, "Failed to generate question.\n");
        goto CLEANUP;
    }

CLEANUP:
    if(title != NULL)
    {
        free(title);
        title = NULL;
    }
    if(fd != NULL)
    {
        fclose(fd);
    }

    return retcode;
}

void print_usage(FILE *fout)
{
    fprintf(fout, "Usage: ./generate {output_file}\n");
}
