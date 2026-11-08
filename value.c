#include"value.h"


int determinant_int(const char *box){
    int i = 0;
    int has_digit = 0;
    if(box[i] == '+' || box[i] == '-'){
        i++;
    }
    while(box[i] != '\0'){
        if(box[i] >= '0' && box[i] <= '9'){
            has_digit = 1;
            i++;
        }
        else{
            return 0;
        }
    }
    return has_digit;
}


int determinant_double(const char *box){
    int i = 0;
    int has_before = 0;  // цифры до точки
    int has_after = 0;   // цифры после точки
    int has_dot = 0;     // была ли точка

    if (box[i] == '+' || box[i] == '-') i++;

    while (box[i] != '\0'){

        if (box[i] >= '0' && box[i] <= '9'){

            if (has_dot == 0) {
                has_before = 1;  // цифра до точки
            } else {
                has_after = 1;   // цифра после точки
            }
            i++;
        }
        else if (box[i] == '.'){

            if (has_dot == 1){
                return 0;   // вторая точка — ошибка
            }
            has_dot = 1;
            i++;
        }
        else{
            return 0;   // посторонний символ
        }
    }

    if (has_dot == 1 && has_before == 1 && has_after == 1){
        return 1;
    }
    else{
        return 0;
    }
}


ValueType type_value(const char *box){

    int a = determinant_int(box);
    int b = determinant_double(box);

    if(box[0] == '\0'){
        return TYPE_NULL;
    }
    else if(a == 1){
        return TYPE_INT;
    }
    else if(b == 1){
        return TYPE_DOUBLE;
    }
    else{
        return TYPE_STRING;
    }
}


ValueType type_extend(ValueType old, ValueType new){
    if (old == TYPE_STRING || new == TYPE_STRING) return TYPE_STRING;
    if (old == TYPE_DOUBLE || new == TYPE_DOUBLE) return TYPE_DOUBLE;
    if (old == TYPE_INT || new == TYPE_INT) return TYPE_INT;
    return TYPE_NULL;
}


