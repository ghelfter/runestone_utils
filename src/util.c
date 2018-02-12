/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                     util.c
 **************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "util.h"

void generate_chomp(char *buffer, int buffer_len, char delim)
{
    int len = strnlen(buffer, buffer_len);
    int m_off = (len-1) < 0 ? 0 : (len-1);

    if(*(buffer+m_off) == delim)
    {
        *(buffer+m_off) = '\0';
    }
}

void prompt_and_chomp(const char *prompt, char *buffer, int size,
                      FILE *infile)
{
    fprintf(stdout, "%s", prompt);
    fgets(buffer, size, infile);
    generate_chomp(buffer, size, '\n');
}
