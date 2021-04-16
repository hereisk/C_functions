#include "my_tar.h"

/*
0 tar (funciton call)
1 options
2 archive name
3 arvhice memebers

accaptable option arragements for tar:
-cf
c -f
-c -f
*/

int input_error(void)
{
    char *error_format = "Please verify the format: './tar -[option]f [archive name] [archive members...]";
    write(1, error_format, _strlen(error_format));

    return 1;
}

int main(int count, char** input)
{
    if(count < 2)
    {
        return input_error();
    }

    if(_strcmp(input[1], MODE_C) == 0)
    {
        return mode_c(count, input);
    } else if(_strcmp(input[1], MODE_X) == 0)
    {
        return mode_x(input);
    } else if(_strcmp(input[1], MODE_R) == 0)
    {
        return mode_r(count, input);
    } else if(_strcmp(input[1], MODE_T) == 0)
    {
        return mode_ls(input);
    } else if(_strcmp(input[1], MODE_U) == 0)
    {
        return mode_u(count, input);
    } else
    {
        return input_error();
    }

    return 1;
}   