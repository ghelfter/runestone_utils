/**************************************************
 *                  Galen Helfter
 *               ghelfter@gmail.com
 *                  gen_function.h
 **************************************************/

#ifndef _GEN_FUNCTION_H_
#define _GEN_FUNCTION_H_

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

typedef int (*question_func_t)(FILE *fd, FILE *infile, const char *file);

#endif /* _GEN_FUNCTION_H_ */
