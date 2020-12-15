#include <stdio.h>// <<<--- for printf, can be deleted
#include <stdarg.h>// library for va_args functions
#include <unistd.h>// library for write()

int _strlen(char *string)
// returns the total length of a string
{
    int i = 0;
    while(string[i] != '\0')
    {
      i++;
    }
    return i;
}

int numlen (long number, int base)
// to count how many digits is there in the number
{
    int count = 0;
    while (number > base)
    {
        number = number / base;
        count++;
    }
    count++;
    return count;
}

void _itoa(long number, int base)
// to prints out a numbers as a string
{
    char *conversion_string = "0123456789abcdef";
    if (number > base)
    {
        _itoa(number / base, base);
        write(1, &conversion_string[number % base], 1);

    } else
    {
        write(1, &conversion_string[number % base], 1);
    }
}

int my_printf(char* string, ...)
{
    char *s;
    int i;
    int output_length = 0;// this variable tracks the total length of a printed output
    int index;
    long li;
    unsigned int ui;
    va_list args;
    va_start(args, string);
    for(index = 0; string[index] != '\0'; index++)
    {
        if(string[index] != '%')
        {
            // the string is getting printed out until it reaches the flag %
            write(1, &string[index], 1);
            output_length++;
        }
        if(string[index] == '%')
        // once the flag is reached a special case is evaluated
        {
            index++;
            switch(string[index])
            {
            case 's':// a handle to print a string
                s = va_arg(args, char *);
                if(s)
                {
                    write(1, s, _strlen(s));
                    output_length += _strlen(s);
                } else
                {
                    write(1, "(null)", 6);
                    output_length += 6;
                }
                break;

            case 'c':// a handle to print a character
                i = va_arg(args, int);
                if(i)
                {
                    write(1, &i, 1);
                    output_length++;
                } else
                {
                    write(1, "(null)", 6);
                    output_length += 6;
                }
                break;

            case 'd':// a handle to print a signed decimal
                i = va_arg(args, int);
                if (i < 0)
                {
                    write(1, "-", 1);
                    i *= -1;
                    output_length++;
                }
                _itoa(i, 10);
                output_length += numlen(i, 10);
                break;

            case 'o':// prints an unsigned octal
                ui = va_arg(args, unsigned int);
                _itoa(ui, 8);
                output_length += numlen(ui, 8);
                break;

            case 'u':// prints an unsigned decimal number
                ui = va_arg(args, unsigned int);
                _itoa(ui, 10);
                output_length += numlen(ui, 10);
                break;

            case 'x':// prints an unsigned hexadecimal
                li = va_arg(args, long);
                _itoa(li, 16);
                output_length += numlen(li, 16);
                break;

            case 'p':// prints a memory address as a string
                li = va_arg(args, long);
                write(1, "0x", 2);
                output_length += 2;
                _itoa(li, 16);
                output_length += numlen(li, 16);
                break;
            }
        }
    }
    va_end(args);
    printf("%i\n", output_length); //<< --- this is for the ease of testing to check the correct return
    return output_length;
}

int main(void)
{
    int p;
    //printf("%p\n", &p); << -- just to compare the output with my_printf
    /*
    my_printf("Hello world!\n");
    my_printf("%c!\n", 'H');
    my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
    my_printf("\n");
    my_printf("Hello %s!\n", "QWASAR.IO");
    my_printf("Hello %s %s!\n", "Mr", "Gaetan");
    my_printf("NULL STRING %s!\n", (char*)NULL);
    my_printf("\n");
    
    my_printf("%d!\n", 1337);
    my_printf("%d - %d - %d!\n", 2048, 0, -1337);
    my_printf("\n");
    my_printf("%o!\n", 100);
    my_printf("\n");
    my_printf("%u!\n", 1337);
    my_printf("\n");
    my_printf("%x!\n", 14);
    */

    my_printf("%d - %d - %d!\n", 2048, 0, -1337);
    my_printf("\n");
    my_printf("%p!\n", &p);
    
    return 0;
}