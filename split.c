#include "split.h"
#include<stdlib.h>
#include<string.h>



//убираем пробелы и табуляции из строки
void trim(char *s){
    int len = (int)strlen(s);
    int start = 0;
    int end = 0;
    int i = 0;
    int j = 0;
    while((i < len) && (s[i] == ' ' || s[i] == '\t')){
        i++;
    }
    start = i;
    while((j<len) && (s[len - 1 - j] == ' ' || s[len - 1 - j] == '\t')){
        j++;
    }
    end = len - 1 -j;
    if(start > end){
        s[0] = '\0';
        return;
    }
    memmove(s,s + start,end - start + 1);
    s[end - start + 1] = '\0';
}



//освобождает память
void free_fields(char **fields, size_t count) {
    if (fields == NULL) return;
    for (size_t i = 0; i < count; i++) free(fields[i]);
    free(fields);
}



//устанавливает все значения внутри структуры по нулям
void char_box_init(CharBox *box){
    box->symbols = NULL;
    box->len = 0;
    box->capacity = 0;
}



// кладет символ в массив
int char_box_push(CharBox *box, char a){
    if(box->len >= box->capacity){
        size_t new_cap;
        if(box->capacity == 0){
            new_cap = 16;
        }
        else{
            new_cap = box->capacity * 2;
        }
        char *new_box;
        new_box = realloc(box->symbols, new_cap);

        if (new_box == NULL){
            return -1;
        }
        box->symbols = new_box;
        box->capacity = new_cap;
    }
    box->symbols[box->len] = a;
    box->len ++;
    return 0;
}



//копирует в отдельный указатель и ставит \0 на конце 
char *char_box_string(CharBox *box){
    char *copy = malloc(box->len + 1);
    if (copy == NULL){
        return NULL;
    }
    memcpy(copy, box->symbols, box->len);
    copy[box->len] = '\0';
    return copy;
}



//нарезает массив по запятым и выдает массив из массивов
char **char_box_cut(char *copy, size_t *count){
    char **BOX = NULL;
    CharBox box;
    size_t words = 0;
    size_t box_cap = 0;
    char_box_init(&box);
    int i = 0;
    while(copy[i] != '\0'){
        char c = copy[i];
        if(c == ',' || c == '\n'){
            if(box.len > 0){
                while (words >= box_cap) {
                    if(box_cap == 0){
                        box_cap = 16;
                    }
                    box_cap = box_cap * 2;
                    char **tmp = realloc(BOX, box_cap * sizeof(char *));
                    if (tmp == NULL){
                        free_fields(BOX, words);
                        free(box.symbols);
                        return NULL;
                    }
                    BOX = tmp;
                }
                BOX[words] = char_box_string(&box);
                if (BOX[words] == NULL){
                    free_fields(BOX, words);
                    free(box.symbols);
                    return NULL;
                }
                trim(BOX[words]);
                words++;

                
            }
            box.len = 0;  
        }
        else{
            if (char_box_push(&box, copy[i]) != 0) {
                free_fields(BOX, words);
                free(box.symbols);
                return NULL;
            }
        }
        i++;
    }
    while (words >= box_cap) {
        if(box_cap == 0){
            box_cap = 16;
        }
        box_cap = box_cap * 2;
        char **tmp = realloc(BOX, box_cap * sizeof(char *));
        if (tmp == NULL){
            free_fields(BOX, words);
            free(box.symbols);
            return NULL;
        }
        BOX = tmp;
    }
    if (box.len != 0){
        BOX[words] = char_box_string(&box);
        if (BOX[words] == NULL) {
            free_fields(BOX, words);
            free(box.symbols);
            return NULL;
        }
        trim(BOX[words]);
        words++;
    }
    *count = words;
    free (box.symbols);
    return (BOX);
}



char **cut_in_header(char *copy, size_t *count){
    char **BOX = NULL;
    CharBox box;
    size_t words = 0;
    size_t box_cap = 0;
    char_box_init(&box);
    int i = 0;
    while(copy[i] != '\0'){
        char c = copy[i];
        if(c == ','){
            if(box.len > 0){
                while (words >= box_cap) {
                    if(box_cap == 0){
                        box_cap = 16;
                    }
                    box_cap = box_cap * 2;
                    char **tmp = realloc(BOX, box_cap * sizeof(char *));
                    if (tmp == NULL){
                        free_fields(BOX, words);
                        free(box.symbols);
                        return NULL;
                    }
                    BOX = tmp;
                }
                BOX[words] = char_box_string(&box);
                if (BOX[words] == NULL){
                    free_fields(BOX, words);
                    free(box.symbols);
                    return NULL;
                }
                trim(BOX[words]);
                words++;

            }
            box.len = 0;
        }
        else{
            if (char_box_push(&box, copy[i]) != 0) {
                free_fields(BOX, words);
                free(box.symbols);
                return NULL;
            }
        }
        i++;
    }
    while (words >= box_cap) {
        if(box_cap == 0){
            box_cap = 16;
        }
        box_cap = box_cap * 2;
        char **tmp = realloc(BOX, box_cap * sizeof(char *));
        if (tmp == NULL){
            free_fields(BOX, words);
            free(box.symbols);
            return NULL;
        }
        BOX = tmp;
    }
    if (box.len != 0){
        BOX[words] = char_box_string(&box);
        if (BOX[words] == NULL) {
            free_fields(BOX, words);
            free(box.symbols);
            return NULL;
        }
        trim(BOX[words]);
        words++;
    }
    *count = words;
    free (box.symbols);
    return (BOX);
}

