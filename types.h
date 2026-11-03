#ifndef TYPES_H
#define TYPES_H

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} ValueType;

/*
 * определяет тип отдельного значения по строке
 */
ValueType types_detect(const char *s);

/*
 * Возвращает строковое имя типа ("INT", "DOUBLE", "STRING").
 */
const char *types_name(ValueType t);


ValueType types_raise(ValueType column_type, ValueType value_type);

#endif /* TYPES_H */