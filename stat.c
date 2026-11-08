#include"stat.h"
#include "split.h"
#include "value.h"
#include"schema.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>


void stats_update_int(IntStats *s, const char *field) {

    int64_t val = strtoll(field, NULL, 10);

    if (!s->has) {                   // первое значение
        s->min = val;
        s->max = val;
        s->has = 1;
    } else {
        if (val < s->min) s->min = val;
        if (val > s->max) s->max = val;
    }

    s->sum += (long double)val;
    if (val > 0) s->positive++;
    if (val < 0) s->negative++;
    if (val % 2 == 0) s->even++;
}


void stats_update_double(DoubleStats *s, const char *field){
    if (field[0] == '\0') {          // пусто → NULL
        return;
    }

    double val = strtod(field, NULL);

    if (!s->has) {                   // первое значение
        s->min = val;
        s->max = val;
        s->has = 1;
    } else {
        if (val < s->min) s->min = val;
        if (val > s->max) s->max = val;
    }

    s->sum += (long double)val;
    if (val > 0) s->positive++;
    if (val < 0) s->negative++;
}


ColumnStats *stats_compute(Schema *s, const char *filename) {
    // 1. Выделить массив статистик
    ColumnStats *st = calloc(s->ncol, sizeof(ColumnStats));
    if (st == NULL) return NULL;

    // 2. Прочитать файл
    char *content = read(filename);
    if (content == NULL) {
        free(st);
        return NULL;
    }

    // 3. Пропустить заголовок
    char *nl = strchr(content, '\n');
    char *data_start;
    if (nl != NULL) {
        data_start = nl + 1;
    }
    else {
        data_start = content + strlen(content);
    }

    // 4. Разрезать данные
    size_t data_count = 0;
    char **data = NULL;
    if(*data_start != '\0'){
        data = char_box_cut(data_start, &data_count);
        if(data == NULL){
            free(content);
            free(st);
            return NULL;
        }
    }

    // 5. Проверка: число полей совпадает со схемой

    if(data_count != s->nrow * s->ncol){
        free_fields(data, data_count);
        free(content);
        free(st);
        return NULL;
    }

    // 6. Запомнить типы столбцов
    for(size_t j = 0; j < s->ncol; j++){
        st[j].type = s->columns[j].type;
    }

    // 7. Пройти по полям
    for(size_t i = 0; i < s->nrow; i++){
        for (size_t j = 0; j < s->ncol; j++) {
            const char *field = data[i * s->ncol + j];

            if (field[0] == '\0') {
                st[j].nulls++;
                continue;
            }

            if (s->columns[j].type == TYPE_INT) {
                stats_update_int(&st[j].i, field);
            }
            else if (s->columns[j].type == TYPE_DOUBLE) {
                stats_update_double(&st[j].d, field);
            }
        }
    }

    // 8. Освободить временное
    free_fields(data, data_count);
    free(content);

    return st;
}


void stats_free(ColumnStats *st){
    free(st);
}


void stats_print(const Schema *s, const ColumnStats *st, FILE *out) {
    fprintf(out, "rows %zu\n", s->nrow);
    fprintf(out, "columns %zu\n", s->ncol);

    for (size_t j = 0; j < s->ncol; j++) {
        fputc('\n', out);

        fprintf(out, "column %zu name ", j);
        print_quoted_name(out, s->columns[j].name);

        if (s->columns[j].type == TYPE_INT) {
            fprintf(out, " type INT nulls %zu", st[j].nulls);
            if (st[j].i.has) {
                fprintf(out, " min %lld max %lld",
                        (long long)st[j].i.min, (long long)st[j].i.max);
                long double mean = st[j].i.sum / (long double)(s->nrow - st[j].nulls);
                if (mean == 0.0L) mean = 0.0L;
                fprintf(out, " mean %.6Lf", mean);
            }
            fprintf(out, " positive %zu negative %zu even %zu\n",
                    st[j].i.positive, st[j].i.negative, st[j].i.even);
        }
        else if (s->columns[j].type == TYPE_DOUBLE) {
            fprintf(out, " type DOUBLE nulls %zu", st[j].nulls);
            if (st[j].d.has) {
                double mn = st[j].d.min;
                double mx = st[j].d.max;
                if (mn == 0.0) mn = 0.0;
                if (mx == 0.0) mx = 0.0;
                fprintf(out, " min %.6f max %.6f", mn, mx);
                long double mean = st[j].d.sum / (long double)(s->nrow - st[j].nulls);
                if (mean == 0.0L) mean = 0.0L;
                fprintf(out, " mean %.6Lf", mean);
            }
            fprintf(out, " positive %zu negative %zu\n",
                    st[j].d.positive, st[j].d.negative);
        }
        else if (s->columns[j].type == TYPE_NULL) {
            fprintf(out, " type NULL rows %zu\n", s->nrow);
        }

    }
}