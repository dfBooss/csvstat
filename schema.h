#ifndef SCHEMA_H
#define SCHEMA_H
#include "types.h"
#include "stdio.h"

char **schema_read_header(FILE *file, int *cols);

/* проход по данным: определяет тип каждого столбца
 * по всем значениям. Возвращает количество прочитанных строк данных
 * gри ошибке возвращает -1
 */
long long schema_scan_types(FILE *file, char **col_names, int cols,
                            ValueType *types);

//
void schema_print(long long rows, int cols, char **col_names,
                  const ValueType *types);

#endif