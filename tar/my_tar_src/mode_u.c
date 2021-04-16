#include "my_tar.h"

int compare_names(struct posix_header *file_header, int archive_fd, char *new_archive_member)
{
    // create a check if the file header is valid -> if not, interrupt 
    
    int bytes_to_skip = 0;

    if(_strcmp((*file_header).name, new_archive_member) == 0) // to check the file is already in the archive
    {
        int last_modification_time = octal2uint((*file_header).mtime, HEADER_MTIME_SIZE -1);
        struct stat new_archive_member_info;
        stat(new_archive_member, &new_archive_member_info);
        if(last_modification_time == (int) new_archive_member_info.st_mtime)
        {
        return -1; // indicates that we can stop itirating through the archive            
        }

    } else
    {
        int content_size = octal2uint((*file_header).size, HEADER_SIZE_SIZE -1);
        bytes_to_skip = content_size + (512 - content_size % 512); // calcualtes the size of content + the padding
        lseek(archive_fd, bytes_to_skip, SEEK_CUR);
    }

    return bytes_to_skip;
}

int read_content(int archive_fd, int remaining_content, char *new_archive_member)
{
    while(remaining_content > 0)
    {
    struct posix_header *file_header = malloc(sizeof(struct posix_header));
    remaining_content -= read(archive_fd, file_header, sizeof(struct posix_header));
    int check = compare_names(file_header, archive_fd, new_archive_member);
    if(check == -1)
    {
        return 0;
    } else
    {
        remaining_content -= check;
    }
    free(file_header);
    }

    return process_archive_member(archive_fd, new_archive_member);
}

int mode_u(int count, char** input)
{
    char* archive_filename = input[2];
    char** files_to_process = (char **) &input[3];
    int number_of_files = count - 3;

    if(ifexists(archive_filename) == 0)
    {
        for(int i = 0; i < number_of_files; i++)
        {
            if(ifexists(files_to_process[i]) == 0)
            {
                struct stat archive_info;
                stat(archive_filename, &archive_info);
                int remaining_content = archive_info.st_size;
                int archive_fd = open(archive_filename, O_RDWR);
                if (read_content(archive_fd, remaining_content, files_to_process[i]) == 0)
                {
                    close(archive_fd);
                    return 0;
                }
                close(archive_fd);
            }
            else
            {
                return 1;
            }
        }
        return 0;
    } else
    {
        char *message = "Cannot read the archive or archive doesn't exist\n";
        write(1, message, _strlen(message));

        return 1;
    }
    return 0;
}