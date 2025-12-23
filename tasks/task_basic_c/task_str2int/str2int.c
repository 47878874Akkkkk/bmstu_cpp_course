#include "str2int.h"
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

int str2int(const char* str)
{
    int result = 0;
    int sign = 1;
    int i = 0;
    
    while (str[i] == ' ') {
        i++;
    }
    
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}