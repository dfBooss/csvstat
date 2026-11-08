#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stats_init(ColumnStats *st, ValueType type, long long count) {
    memset(st, 0, sizeof(*st));
    st->type = type;
    st->count = count;
}

void stats_update(ColumnStats *st, const char *value, int is_first) {
    if (st->type == TYPE_INT) {
        long long v = strtoll(value, NULL, 10);

        if (is_first) {
            st->int_min = v;
            st->int_max = v;
        } else {
            if (v < st->int_min) st->int_min = v;
            if (v > st->int_max) st->int_max = v;
        }

        st->int_sum += v;
        if (v > 0) st->int_positive++;
        if (v < 0) st->int_negative++;
        if (v % 2 == 0) st->int_even++;

    } else if (st->type == TYPE_DOUBLE) {
        double v = strtod(value, NULL);

        if (is_first) {
            st->double_min = v;
            st->double_max = v;
        } else {
            if (v < st->double_min) st->double_min = v;
            if (v > st->double_max) st->double_max = v;
        }

        st->double_sum += v;
        if (v > 0.0) st->double_positive++;
        if (v < 0.0) st->double_negative++;

    } else if (st->type == TYPE_STRING) {
        int len = (int)strlen(value);
        if (is_first) {
            st->str_min_len = len;
            st->str_max_len = len;
        } else {
            if (len < st->str_min_len) st->str_min_len = len;
            if (len > st->str_max_len) st->str_max_len = len;
        }
        st->str_sum_len += len;
        if (len == 0) st->str_empty++;
    }
}

void stats_print(const ColumnStats *st, int index, const char *name,
                 long long rows) {
    /* Заголовок блока: одна строка */
    printf("column %d name \"", index);
    types_print_escaped(name);
    printf("\" type %s\n", types_name(st->type));
    printf("nulls 0\n");

    if (st->type == TYPE_INT) {
        printf("min %lld\n", st->int_min);
        printf("max %lld\n", st->int_max);
        double mean = (rows > 0) ? (double)st->int_sum / rows : 0.0;
        printf("mean %.6f\n", mean);
        printf("positive %lld\n", st->int_positive);
        printf("negative %lld\n", st->int_negative);
        printf("even %lld\n", st->int_even);

    } else if (st->type == TYPE_DOUBLE) {
        printf("min %.6f\n", st->double_min);
        printf("max %.6f\n", st->double_max);
        double mean = (rows > 0) ? st->double_sum / rows : 0.0;
        printf("mean %.6f\n", mean);
        printf("positive %lld\n", st->double_positive);
        printf("negative %lld\n", st->double_negative);

    } else if (st->type == TYPE_STRING) {
        printf("min_length %d\n", st->str_min_len);
        printf("max_length %d\n", st->str_max_len);
        double mean_len = (rows > 0) ? (double)st->str_sum_len / rows : 0.0;
        printf("mean_length %.6f\n", mean_len);
        printf("empty %lld\n", st->str_empty);
    }
}