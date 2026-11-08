#ifndef VALUE_H
#define VALUE_H




typedef enum{
    TYPE_NULL,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
}ValueType;


int determinant_int(const char *box);
int determinant_double(const char *box);
ValueType type_value(const char *box);
ValueType type_extend(ValueType old, ValueType new);


#endif