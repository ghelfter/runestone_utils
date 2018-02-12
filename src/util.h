/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                     util.h
 **************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>

#define MAX(x,y) ((x) > (y) ? (x) : (y))

void generate_chomp(char *buffer, int buffer_len, char delim);
void prompt_and_chomp(const char *prompt, char *buffer, int size, 
                      FILE *infile);

#endif
