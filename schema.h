#ifndef SCHEMA_H
#define SCHEMA_H
#include<stddef.h>
#include<stdio.h>
#include "split.h"
#include "value.h"


typedef struct{
    char *name;
    ValueType type;
}Column;


typedef struct{
    size_t ncol;
    size_t nrow;
    Column *columns;
}Schema;


char *read(const char *filename);
Schema *schema_build(const char *);
void schema_free(Schema *s);
void print_quoted_name(FILE *out, const char *name);
void schema_print(const Schema *s, FILE *out);

#endif