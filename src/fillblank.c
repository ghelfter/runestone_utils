/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                    mchoice.c
 **************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "gen_function.h"
#include "generate.h"
#include "util.h"

#define MAX_BUFFER_SIZE 128
#define MAX_BIG_BUFFER_SIZE 4096

int generate_fill_in_blanks(FILE *fd, FILE *infile, const char *title)
{
    int retcode = GENERATE_SUCCESS;

    char read_buff[MAX_BIG_BUFFER_SIZE];

    if(fd == NULL || infile == NULL || title == NULL)
    {
        retcode = GENERATE_NULL_PTR;
        goto CLEANUP;
    }

    fputs("Note: You will need to add the |blank| fields where you want them"
          " when typing in the question. These will put the blanks where you"
          " want.\n", stdout);

    prompt_and_chomp("Please enter the number of blanks you will have: ",
                     read_buff, MAX_BIG_BUFFER_SIZE, infile);

CLEANUP:
    return retcode;
}
