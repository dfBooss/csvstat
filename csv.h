#ifndef CSV_H
#define CSV_H

#include <stdio.h>

char *csv_read_line(FILE *file);

char **csv_split_line(const char *line, int *count);

void csv_free_fields(char **fields, int count);

#endif /* CSV_H */