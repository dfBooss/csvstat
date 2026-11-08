#include "schema.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **schema_read_header(FILE *file, int *cols) {
    char *line = csv_read_line(file);
    if (!line) return NULL;

    int n = 0;
    char **names = csv_split_line(line, &n);
    free(line);
    if (!names) return NULL;

    *cols = n;
    return names;
}

long long schema_scan_types(FILE *file, char **col_names, int cols,
                            ValueType *types) {
    (void)col_names;

    for (int i = 0; i < cols; i++) {
        types[i] = TYPE_INT;
    }

    long long rows = 0;
    char *line;
    while ((line = csv_read_line(file)) != NULL) {
        int fc = 0;
        char **fields = csv_split_line(line, &fc);
        free(line);
        if (!fields) return -1;

        if (fc != cols) {
            csv_free_fields(fields, fc);
            continue;
        }

        for (int i = 0; i < cols; i++) {
            ValueType t = types_detect(fields[i]);
            types[i] = types_raise(types[i], t);
        }

        csv_free_fields(fields, fc);
        rows++;
    }

    return rows;
}

void schema_print(long long rows, int cols, char **col_names,
                  const ValueType *types) {
    printf("rows %lld columns %d\n", rows, cols);
    for (int i = 0; i < cols; i++) {
        printf("%d \"", i);
        types_print_escaped(col_names[i]);
        printf("\" %s\n", types_name(types[i]));
    }
}