/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                   generate.h
 **************************************************/

#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>

/* Question type integer codes */
#define QUESTION_TYPE_NONE         0
#define QUESTION_TYPE_MULTI_CHOICE 1
#define QUESTION_TYPE_FILLIN_BLANK 2
#define QUESTION_TYPE_SHORT_ANSWER 3

/* Return codes */
#define GENERATE_SUCCESS       0
#define GENERATE_NULL_PTR      1
#define GENERATE_FOPEN_FAIL    2
#define GENERATE_FWRITE_FAIL   3
#define GENERATE_BAD_CODE      4
#define GENERATE_PARSE_ERROR   5
#define GENERATE_INVALID_NUM   6
#define GENERATE_BAD_ALLOC     7
#define GENERATE_INVALID_LABEL 8

/* Function definitions */
int generate_question_type(FILE *fd, int question_type, const char *title);
void print_question_descriptions(FILE *fd);
int generate_question(FILE *fd, int question_typ, const char *title);
int acquire_question_type(FILE *fd, int *in_qtype);
int acquire_question_id(FILE *fd, char *buffer, int buffer_size);

#endif
