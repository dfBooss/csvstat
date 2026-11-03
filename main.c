#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csv.h"
#include "types.h"
#include "stats.h"
#include "schema.h"

static void print_help(void) {
    printf("Usage:\n");
    printf("  csvstat --help\n");
    printf("  csvstat schema <csv-file>\n");
    printf("  csvstat stats <csv-file>\n");
}

/* команда schema */
static int cmd_schema(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }

    int cols = 0;
    char **names = schema_read_header(f, &cols);
    if (!names) {
        fclose(f);
        return 1;
    }

    ValueType *types = malloc(cols * sizeof(ValueType));
    if (!types) {
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }

    long long rows = schema_scan_types(f, names, cols, types);
    if (rows < 0) {
        free(types);
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }

    schema_print(rows, cols, names, types);

    free(types);
    csv_free_fields(names, cols);
    fclose(f);
    return 0;
}

/* команда stats. */
static int cmd_stats(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }

    int cols = 0;
    char **names = schema_read_header(f, &cols);
    if (!names) {
        fclose(f);
        return 1;
    }

    ValueType *types = malloc(cols * sizeof(ValueType));
    if (!types) {
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }

    /* типы столбцов */
    long long rows = schema_scan_types(f, names, cols, types);
    if (rows < 0) {
        free(types);
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }

    /* инициализация статистики */
    ColumnStats *st = malloc(cols * sizeof(ColumnStats));
    if (!st) {
        free(types);
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }
    for (int i = 0; i < cols; i++) {
        stats_init(&st[i], types[i], rows);
    }

    /* обновление статистики */
    fseek(f, 0, SEEK_SET);
    char *line = csv_read_line(f);
    free(line);

    int *first = malloc(cols * sizeof(int));
    if (!first) {
        free(st);
        free(types);
        csv_free_fields(names, cols);
        fclose(f);
        return 1;
    }
    for (int i = 0; i < cols; i++) first[i] = 1;

    while ((line = csv_read_line(f)) != NULL) {
        int fc = 0;
        char **fields = csv_split_line(line, &fc);
        free(line);
        if (!fields) break;

        if (fc != cols) {
            csv_free_fields(fields, fc);
            continue;
        }

        for (int i = 0; i < cols; i++) {
            stats_update(&st[i], fields[i], first[i]);
            first[i] = 0;
        }
        csv_free_fields(fields, fc);
    }

    /* вывод */
    printf("rows %lld\n", rows);
    printf("columns %d\n", cols);
    for (int i = 0; i < cols; i++) {
        if (i > 0) printf("\n");
        stats_print(&st[i], i, names[i], rows);
    }

    free(first);
    free(st);
    free(types);
    csv_free_fields(names, cols);
    fclose(f);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }
    if (argc == 3 && strcmp(argv[1], "schema") == 0) {
        return cmd_schema(argv[2]);
    }
    if (argc == 3 && strcmp(argv[1], "stats") == 0) {
        return cmd_stats(argv[2]);
    }

    fprintf(stderr, "Invalid arguments. Use --help for usage.\n");
    return 2;
}