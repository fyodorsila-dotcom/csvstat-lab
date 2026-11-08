#include"schema.h"
#include"split.h"
#include"value.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>




char *read(const char *filename){
    CharBox box;
    char_box_init(&box);
    FILE *f = fopen(filename, "r");
        if (f == NULL){
            return NULL;
        }
        int c;
        while ((c = fgetc(f)) != EOF){
            int a = char_box_push(&box, c);
            if(a != 0){
                free(box.symbols);
                fclose(f);
                return NULL;
            }
        }
        char *file = char_box_string(&box);
        if(file == NULL){
            free(box.symbols);
            fclose(f);
            return NULL;
        }
        fclose(f);
        free(box.symbols);
    return(file);
}


Schema *schema_build(const char *filename){
    char *content = read(filename);
    if(content == NULL){
        return NULL;
    }

    char *nl = strchr(content, '\n');
    size_t header_len;
    if(nl != NULL){
        header_len = nl - content;
    }
    else{
        header_len = strlen(content);
    }

    char *header = malloc(header_len + 1);
    if(header == NULL){
        free(content);
        return NULL;
    }
    memcpy(header, content, header_len);
    header[header_len] = '\0';

    size_t ncols;
    char **names = cut_in_header(header, &ncols);
    free(header);
    if(names == NULL){
        free(content);
        return NULL;
    }



    char *data_start;
    if (nl != NULL) {
        data_start = nl + 1;
    } else {
        data_start = content + strlen(content);
    }

    size_t words = 0;
    char **value = NULL;
    if (*data_start != '\0') {
        value = char_box_cut(data_start, &words);
        if (value == NULL) {
            free_fields(names, ncols);
            free(content);
            return NULL;
        }
    }

    if (ncols == 0 || words % ncols != 0) {
        free_fields(names, ncols);
        free_fields(value, words);
        free(content);
        return NULL;
    }

    size_t nrows = words / ncols;

    Schema *s = malloc(sizeof(Schema));
    if(s == NULL){
        free_fields(names, ncols);
        free_fields(value, words);
        free(content);
        return NULL;
    }
    s->ncol = ncols;
    s->nrow = nrows;
    s->columns = malloc(ncols * sizeof(Column));
    if(s->columns == NULL){
        free_fields(names, ncols);
        free_fields(value, words);
        free(content);
        return NULL;
    }


    for (size_t j = 0; j < ncols; j++) {
        s->columns[j].name = strdup(names[j]);
        s->columns[j].type = TYPE_NULL;
        for (size_t i = 0; i < nrows; i++) {
            const char *field = value[i * ncols + j];
            s->columns[j].type = type_extend(s->columns[j].type, type_value(field));
        }
    }
    free_fields(names, ncols);
    free_fields(value, words);
    free(content);

    return s;
}


void schema_free(Schema *s) {
    if (s == NULL) return;
    for (size_t i = 0; i < s->ncol; i++) {
        free(s->columns[i].name);
    }
    free(s->columns);
    free(s);
}


void print_quoted_name(FILE *out, const char *name) {
    fputc('"', out);
    for (size_t k = 0; name[k] != '\0'; k++) {
        if (name[k] == '"') fputc('"', out);
        fputc(name[k], out);
    }
    fputc('"', out);
}


void schema_print(const Schema *s, FILE *out) {
    fprintf(out, "rows %zu columns %zu\n", s->nrow, s->ncol);
    for (size_t i = 0; i < s->ncol; i++) {
        fprintf(out, "%zu ", i);
        print_quoted_name(out, s->columns[i].name);
        fprintf(out, " ");
        switch (s->columns[i].type) {
            case TYPE_NULL:   fprintf(out, "NULL");   break;
            case TYPE_INT:    fprintf(out, "INT");    break;
            case TYPE_DOUBLE: fprintf(out, "DOUBLE"); break;
            case TYPE_STRING: fprintf(out, "STRING"); break;
        }
        fputc('\n', out);
    }
}


