#include "csv.h"
#include <stdlib.h>
#include <string.h>

char *csv_read_line(FILE *file) {
    size_t capacity = 16;
    size_t length = 0;
    char *buffer = malloc(capacity);
    if (!buffer) return NULL;

    int ch;
    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *tmp = realloc(buffer, capacity);
            if (!tmp) {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
        }
        buffer[length++] = (char)ch;
    }

    if (ch == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

char **csv_split_line(const char *line, int *count) {
    int n = 1;
    for (const char *p = line; *p; p++) {
        if (*p == ',') n++;
    }

    char **fields = malloc(n * sizeof(char *));
    if (!fields) return NULL;

    int idx = 0;
    const char *start = line;
    for (const char *p = line; ; p++) {
        if (*p == ',' || *p == '\0') {
            size_t len = p - start;
            char *field = malloc(len + 1);
            if (!field) {
                for (int i = 0; i < idx; i++) free(fields[i]);
                free(fields);
                return NULL;
            }
            memcpy(field, start, len);
            field[len] = '\0';
            fields[idx++] = field;

            if (*p == '\0') break;
            start = p + 1;
        }
    }

    *count = n;
    return fields;
}
void csv_free_fields(char **fields, int count) {
    if (!fields) return;
    for (int i = 0; i < count; i++) free(fields[i]);
    free(fields);
}