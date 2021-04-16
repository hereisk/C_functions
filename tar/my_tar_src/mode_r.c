#include "my_tar.h"

int mode_r(int count, char** input)
{

    char* archive_filename = input[2];
    char** files_to_process = (char **) &input[3];
    int number_of_files = count - 3;

    int archive_fd = open(archive_filename, O_RDWR);
    lseek(archive_fd, 0, SEEK_END);

    for(int i = 0; i < number_of_files; i++)
    {
        if(ifexists(files_to_process[i]) == 0)
        {
            process_archive_member(archive_fd, files_to_process[i]);
        }
        else
        {
            return 1;
        }
    }

    return 0;
}