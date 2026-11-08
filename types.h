#ifndef TYPES_H
#define TYPES_H

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} ValueType;

ValueType types_detect(const char *s);
const char *types_name(ValueType t);
ValueType types_raise(ValueType column_type, ValueType value_type);
void types_print_escaped(const char *s);

#endif