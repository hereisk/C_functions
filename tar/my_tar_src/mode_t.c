#include "my_tar.h"

int print_name(char *archive_member_name)
{
    write(1, archive_member_name, _strlen(archive_member_name));
    write(1, "\n", 2);
    return 0;
}

int read_file_header(struct posix_header *file_header, int archive_fd)
{
    // create a check if the file header is valid -> if not, interrupt 
    
    int bytes_to_skip = 0;

    if((*file_header).typeflag == '0')
    {
        int content_size = octal2uint((*file_header).size, HEADER_SIZE_SIZE -1);
        bytes_to_skip = content_size + (512 - content_size % 512); // calcualtes the size of content + the padding
        lseek(archive_fd, bytes_to_skip, SEEK_CUR);
    }

    return bytes_to_skip;
}

int list_content(int archive_fd, int remaining_content)
{
    
    while(remaining_content > 0)
    {
    struct posix_header *file_header = malloc(sizeof(struct posix_header));
    remaining_content -= read(archive_fd, file_header, sizeof(struct posix_header));
    remaining_content -= read_file_header(file_header, archive_fd);
    free(file_header);
    }

    return 0;
}

int mode_ls(char** input) // this is mode -t for tar
{
    char* archive_filename = input[2];  
    
    if(ifexists(archive_filename) == 0)
    {
        struct stat archive_info;
        stat(archive_filename, &archive_info);
        int remaining_content = archive_info.st_size;
        int archive_fd = open(archive_filename, O_RDONLY);
        
        list_content(archive_fd, remaining_content);
        close(archive_fd);
        return 0;
    } else
    {
        printf("Cannot read the archive");

        return 1;
    }
    return 0;
}