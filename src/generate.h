/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                   generate.h
 **************************************************/

#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>

/* Function definitions */
int generate_question_type(FILE *fd, int question_type, const char *title);
void print_question_descriptions(FILE *fd);
int generate_question(FILE *fd, int question_typ, const char *title);
int acquire_question_type(FILE *fd, int *in_qtype);
int acquire_question_id(FILE *fd, char *buffer, int buffer_size);

#endif
