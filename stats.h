#ifndef STATS_H
#define STATS_H

#include "types.h"


typedef struct {
    ValueType type;
    long long count;         // количество строк

    // INT
    long long int_min;
    long long int_max;
    long long int_sum;
    long long int_positive;
    long long int_negative;
    long long int_even;

    // DOUBLE
    double double_min;
    double double_max;
    double double_sum;
    long long double_positive;
    long long double_negative;

    // STRING
    int str_min_len;
    int str_max_len;
    long long str_sum_len;
    long long str_empty;
} ColumnStats;

/*
 * статистикf для столбца указанного типа
 */
void stats_init(ColumnStats *st, ValueType type, long long count);


void stats_update(ColumnStats *st, const char *value, int is_first);

void stats_print(const ColumnStats *st, int index, const char *name,
                 long long rows);

#endif