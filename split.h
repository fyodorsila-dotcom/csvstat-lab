#ifndef SPLIT_H
#define SPLIT_H

#include<stddef.h>


typedef struct{
    char *symbols;
    size_t len;
    size_t capacity;
}CharBox;


void free_fields(char **fields, size_t count);
void char_box_init(CharBox *box);
int char_box_push(CharBox *box, char a);
char *char_box_string(CharBox *box);
char **char_box_cut(char *copy, size_t *count);
char **cut_in_header(char *copy, size_t *count);
void trim(char *s);

#endif