#ifndef STAT_H
#define STAT_H
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include "schema.h"
#include "split.h"
#include "value.h"


typedef struct {
    int64_t min;           // минимум
    int64_t max;           // максимум
    long double sum;       // сумма (long double — чтобы не переполнить int64)
    size_t positive;       // сколько > 0
    size_t negative;       // сколько < 0
    size_t even;           // сколько чётных
    int has;               // были ли значения (1/0)
} IntStats;


typedef struct {
    double min;
    double max;
    long double sum;
    size_t positive;
    size_t negative;
    int has;
} DoubleStats;


typedef struct {
    size_t nulls;          // = nrows
} NullStats;


typedef struct {
    ValueType type;
    size_t nulls;
    IntStats    i;
    DoubleStats d;
    NullStats   n;
} ColumnStats;


ColumnStats *stats_compute(Schema *s, const char *filename);
void stats_free(ColumnStats *st);
void stats_update_int(IntStats *s, const char *field);
void stats_update_double(DoubleStats *s, const char *field);
void stats_print(const Schema *s, const ColumnStats *st, FILE *out);

#endif