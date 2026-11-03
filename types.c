#include "types.h"
#include <stdlib.h>
ValueType types_detect(const char *s) {
    if (*s == '\0') return TYPE_STRING;

    const char *p = s;
    if (*p == '+' || *p == '-') p++;

    // INT
    if (!(*p >= '0' && *p <= '9')) return TYPE_STRING;
    while (*p >= '0' && *p <= '9') p++;
    if (*p == '\0') return TYPE_INT;

    // DOUBLE
    if (*p == '.') {
        p++;
        if (!(*p >= '0' && *p <= '9')) return TYPE_STRING;
        while (*p >= '0' && *p <= '9') p++;
        if (*p == '\0') return TYPE_DOUBLE;
    }

    return TYPE_STRING;
}
const char *types_name(ValueType t) {
    switch (t) {
        case TYPE_INT:    return "INT";
        case TYPE_DOUBLE: return "DOUBLE";
        case TYPE_STRING: return "STRING";
        default:          return "NULL";
    }
}

ValueType types_raise(ValueType column_type, ValueType value_type) {
    if (column_type == TYPE_STRING || value_type == TYPE_STRING) {
        return TYPE_STRING;
    }
    if (column_type == TYPE_DOUBLE || value_type == TYPE_DOUBLE) {
        return TYPE_DOUBLE;
    }
    return TYPE_INT;
}