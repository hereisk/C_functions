#include "my_ls.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int my_ls (char* path, int flag_a, int flag_t, int total_folders)
{
    DIR *folder;
    struct dirent *entry;
    struct stat file_info_i;
    struct stat file_info_j;

    struct stat dir_info;
    stat(path, &dir_info);
            
    if(S_ISDIR(dir_info.st_mode)) // if the folder is valid then ls the content is printed out
    {
        if(total_folders > 1)
        {
            printf("%s:\n", path);
        }

        int i;
        char *temp;
        folder = opendir(path);
        struct Content *current_dir;
        current_dir = malloc(sizeof(struct Content));
        current_dir->size = 0;

        while((entry=readdir(folder)))
        {
            current_dir->list[current_dir->size] = entry->d_name;
            current_dir->size++;
        }

        for(int i = 0; i < (current_dir->size - 1); i++) // buble sort of the array in ASCII order
        {
            for(int j = i + 1; j < current_dir->size; j++)
            {
                if(strcmp(current_dir->list[i], current_dir->list[j]) > 0)
                {
                    temp = current_dir->list[i];
                    current_dir->list[i] = current_dir->list[j];
                    current_dir->list[j] = temp;
                }
            }
        }

        if(flag_t == 1) // buble sort base on the time of the last modification
        {   
            long time_i_sec;
            long time_i_nsec;
            long time_j_sec;
            long time_j_nsec;
            for(int i = 0; i < (current_dir->size); i++)
            {
                for(int j = i + 1; j < current_dir->size; j++)
                {
                    lstat(current_dir->list[i], &file_info_i);
                    lstat(current_dir->list[j], &file_info_j);
                    time_i_sec = file_info_i.st_mtim.tv_sec;
                    time_j_sec = file_info_j.st_mtim.tv_sec;
                    time_i_nsec = file_info_i.st_mtim.tv_nsec;
                    time_j_nsec = file_info_j.st_mtim.tv_nsec;
                    
                    if(time_i_sec != time_j_sec)
                    {
                        if(time_i_sec < time_j_sec)
                    {
                        temp = current_dir->list[i];
                        current_dir->list[i] = current_dir->list[j];
                        current_dir->list[j] = temp;
                    }
                    } else if (time_i_nsec != time_j_nsec)
                    {
                        if(time_i_nsec < time_j_nsec)
                        {
                            temp = current_dir->list[i];
                            current_dir->list[i] = current_dir->list[j];
                            current_dir->list[j] = temp;
                        }
                    }
                }
            }
            for(int i = 0; i < (current_dir->size); i++)
            {
                for(int j = i + 1; j < current_dir->size; j++)
                {
                    lstat(current_dir->list[i], &file_info_i);
                    lstat(current_dir->list[j], &file_info_j);
                    time_i_sec = file_info_i.st_mtim.tv_sec;
                    time_j_sec = file_info_j.st_mtim.tv_sec;
                    time_i_nsec = file_info_i.st_mtim.tv_nsec;
                    time_j_nsec = file_info_j.st_mtim.tv_nsec;
                    if(time_i_sec == time_j_sec && time_i_nsec == time_j_nsec)
                    {
                        if(strcmp(current_dir->list[i], current_dir->list[j]) > 0)
                        {
                            temp = current_dir->list[i];
                            current_dir->list[i] = current_dir->list[j];
                            current_dir->list[j] = temp;
                        }
                    }
                }
            }

        }

        if(flag_a == 1) // if -a is the option, then all files are printed out
        {
            for (i = 0; i < current_dir->size; i++)
            {
                printf("%s  ", current_dir->list[i]);
            }
            printf("\n");
        } else // if -a is not the option, then all files that start with '.' (hidden files) are skipped
        {
            for (i = 0; i < current_dir->size; i++)
            {
                if(current_dir->list[i][0] != '.')
                {
                    printf("%s  ", current_dir->list[i]);
                }
            }
            printf("\n");
        }

        closedir(folder);
        free(current_dir);
        return 0;
    } else // if the folder is invalid then a message is printed out and an error is raised
    {
        printf("my_ls: cannot access '%s': No such file or directory", path);
        return 1;
    }
}