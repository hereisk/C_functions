#include "my_tar.h"

/*

-> to check if a file exists
    stat() -> returns 0 if file exists or -1 if it doesn't exist

-> convert octal in char format to int -> bitwise operation here

    • check if the archive exists
    • read the archive
        -> tar header
            -> is the header valid
                -> create a file
                -> assign file stat info as per what's in the header
                    -> convert the information from tar header to relevant formation withing stat()
            -> what is the type of the archive member
                -> if this is a file
                    ->  write the file content

        -> padding of zero blocks -> this is optional
            -> check if it is there

*/

int ifexists(char *path)
{
    struct stat info_struct;
    int returnValue = stat(path, &info_struct);
    if (returnValue == 0 && S_ISREG(info_struct.st_mode)) {
        return returnValue;
    }
    else {
        char *message = "not a file or file doesn't exist\n";
        write(1, message, _strlen(message));
        return 1;
    }
    
    return returnValue; // if the return value is zero, then the file exists. If the retun value is -1, the file does not exist
}

int fill_file_info(struct stat *archive_member_info, struct posix_header *file_header)
{

    (*archive_member_info).st_uid = octal2uint((*file_header).uid, HEADER_UID_SIZE -1);
    (*archive_member_info).st_gid = octal2uint((*file_header).gid, HEADER_GID_SIZE -1);
    (*archive_member_info).st_mtime = octal2uint((*file_header).mtime, HEADER_MTIME_SIZE -1);

    struct passwd *pwd;    
    pwd = getpwuid((*archive_member_info).st_uid);
    _strcpy(pwd->pw_name, (*file_header).uname);
    
    struct group *grp;
    grp = getgrgid((*archive_member_info).st_gid);
    _strcpy(grp->gr_name, (*file_header).gname);

    return 0;
}

int process_file_header(struct posix_header *file_header, int archive_fd)
{
    // create a check if the file header is valid -> if not, interrupt 
    
    int file_fd = open(file_header->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
    int bytes = 0;

    struct stat archive_member_info;
    stat(file_header->name, &archive_member_info);
    fill_file_info(&archive_member_info, file_header);
    if(S_ISREG(archive_member_info.st_mode))
    {
        int content_size = octal2uint((*file_header).size, HEADER_SIZE_SIZE -1);
        char buffer[content_size + (512 - content_size % 512)];
        bytes = read(archive_fd, buffer, (content_size + (512 - content_size % 512)));
        write(file_fd, buffer, content_size);
    }
    
    close(file_fd);
    return bytes;
}

int extract(int archive_fd, int remaining_content)
{
    
    while(remaining_content > 0)
    {
    struct posix_header *file_header = malloc(sizeof(struct posix_header));
    remaining_content -= read(archive_fd, file_header, sizeof(struct posix_header));
    remaining_content -= process_file_header(file_header, archive_fd);
    free(file_header);
    }

    return 0;
}

int mode_x(char** input)
{
    char* archive_filename = input[2];  
    
    if(ifexists(archive_filename) == 0)
    {
        struct stat archive_info;
        stat(archive_filename, &archive_info);
        int remaining_content = archive_info.st_size;
        int archive_fd = open(archive_filename, O_RDONLY);
        
        extract(archive_fd, remaining_content);
        close(archive_fd);
        return 0;
    } else
    {
        char *message = "Cannot read the archive or archive doesn't exist\n";
        write(1, message, _strlen(message));

        return 1;
    }
    return 0;
}