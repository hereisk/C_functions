#include "my_tar.h"

int write_content(int archive_fd, char* archive_member)
{
        int archive_member_size = (int) sizeof(archive_member);
        char buffer[archive_member_size];
        int archive_member_fd = open(archive_member, O_RDONLY);
        int bytes_read = read(archive_member_fd, buffer, archive_member_size);
        write(archive_fd,buffer, bytes_read);
        close(archive_member_fd);

        return bytes_read;
}


int write_end_of_file(int archive_fd, int file_size)
{
    char empty_block[BLOCK * 2] = {0};
    
    return write(archive_fd, empty_block, (512 - (file_size % 512)));
}


int process_archive_member(int archive_fd, char* archive_member)
/*
initiate file_info

    if this is a file
        process_file
            initiate tar_header
            fill in tar_header from file info
            write the tar_header into the archive
            write the file content
*/
{
    struct stat archive_member_info;
    stat(archive_member, &archive_member_info);
    if(S_ISDIR(archive_member_info.st_mode))
    {
        char *error_message = "Working with folders is not supported";
        write(1, error_message, (_strlen(error_message)));
        return 1;

    } else if(S_ISREG(archive_member_info.st_mode))
    {
        
        process_header(archive_fd, archive_member, archive_member_info); //  write the header into the archive
        write_content(archive_fd, archive_member); // write the file content
        write_end_of_file(archive_fd, archive_member_info.st_size);

    } else
    {
        return 1; // an error occurred
    }

    return 0;
}

int mode_c(int count, char** input)
{

    char* archive_filename = input[2];
    char** files_to_process = (char **) &input[3];
    int number_of_files = count - 3;

    int archive_fd = open(archive_filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
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