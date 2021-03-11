#include "my_ls.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char **argv)
{
    int flag_a = 0;
    int flag_t = 0;
    char *sorted_input[argc];
    int total_folders = 0;
    char *temp;

    if (argc > 1) // buble sort of the array from the initially passed command
    {   
        for(int i = 0; i < argc; i++)
        {
            if(argv[i][1] != '/' && argv[i][0] != '-') // copies paths into it's own array
            {
                sorted_input[total_folders] = argv[i];
                total_folders++;
              }
        }
        for(int i = 0; i < (total_folders - 1); i++) // sorting the paths in ASCII order
        {
            for(int j = i + 1; j < total_folders; j++)
            {
                if(strcmp(sorted_input[i], sorted_input[j]) > 0)
                {
                    temp = sorted_input[i];
                    sorted_input[i] = sorted_input[j];
                    sorted_input[j] = temp;
                }
            }
        }
    }

    for(int i = 0; i < argc; i++) // to pick -a and -t flags from the command line
    {
        int j = 0;
        if(argv[i][j] == '-')
        {
            j++;
            while(argv[i][j] != '\0')
            {
                if(argv[i][j] == 'a')
                {
                    flag_a = 1;
                }
                if(argv[i][j] == 't')
                {
                    flag_t = 1;
                }
                if(argv[i][j] != 't' && argv[i][j] != 'a') // if an option is unknown then the error is raised
                {
                    printf("%c is not support. Please use only -a and -t.\n", argv[i][j]);
                    return 1;
                }
                j++;
            }
        }
    }

    if(total_folders == 0) // if no paths are given then the content of the root folder is printed
    {
        my_ls(".", flag_a, flag_t, total_folders);
    } else
    {
        for(int i = 0; i < total_folders; i++)
        {
            my_ls(sorted_input[i], flag_a, flag_t, total_folders);
            if(i < total_folders - 1)
            {
                printf("\n");
            }
        }                
    }
    return(0);
}