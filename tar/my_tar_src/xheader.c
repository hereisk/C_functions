#include "my_tar.h"

void copyOctal(char *dest, unsigned int num, unsigned char size)
{
    char *digits = "01234567";
    int octal_base = 8;
	unsigned char i;
	for (i = 0; i < size - 1; i++) {
		dest[i] = '0';
	}
	dest[i] = 0;
	while (num) {
		dest[--i] = digits[num % octal_base];
		num /= octal_base;
	}
}

unsigned int checksum (char ptr[], int sz, unsigned int chk) {
  int i = 0;
  while( (ptr[i] != '\0') && (i < sz) )
  {
    chk += ptr[i];
    i++;
  }
  return chk;
}

void fill_header(char* archive_member, struct posix_header *tar_header, struct stat archive_member_info)
{
    struct passwd *pwd;    
    pwd = getpwuid(archive_member_info.st_uid);
    
    struct group *grp;
    grp = getgrgid(archive_member_info.st_gid);
    


    _strcpy((*tar_header).name, archive_member);
    copyOctal((*tar_header).mode, (archive_member_info.st_mode & 07777), HEADER_MODE_SIZE);
    copyOctal((*tar_header).uid, archive_member_info.st_uid, HEADER_UID_SIZE);
    copyOctal((*tar_header).gid, archive_member_info.st_gid, HEADER_GID_SIZE);
    copyOctal((*tar_header).size, archive_member_info.st_size, HEADER_SIZE_SIZE);
    copyOctal((*tar_header).mtime, archive_member_info.st_mtime, HEADER_MTIME_SIZE);
    
    switch (archive_member_info.st_mode & S_IFMT)
    {
        case S_IFBLK:  
            (*tar_header).typeflag = BLKTYPE;
            copyOctal((*tar_header).devmajor, major(archive_member_info.st_rdev), HEADER_DEVMAJOR_SIZE);
            copyOctal((*tar_header).devminor, minor(archive_member_info.st_rdev), HEADER_DEVMINOR_SIZE);
            break;
        case S_IFCHR:
            (*tar_header).typeflag = CHRTYPE;
            copyOctal((*tar_header).devmajor, major(archive_member_info.st_rdev), HEADER_DEVMAJOR_SIZE);
            copyOctal((*tar_header).devminor, minor(archive_member_info.st_rdev), HEADER_DEVMINOR_SIZE);
            break;
        case S_IFDIR:
            (*tar_header).typeflag = DIRTYPE;
            break;
        case S_IFIFO:
            (*tar_header).typeflag = FIFOTYPE;
            break;
        case S_IFLNK:
            (*tar_header).typeflag = LNKTYPE;
            if(!readlink((*tar_header).name, (*tar_header).linkname, HEADER_LINKNAME_SIZE))
            {
                printf("ERROR");
            }
            break;
        case S_IFREG:
            (*tar_header).typeflag = REGTYPE;
            break;
        case S_IFSOCK:
            (*tar_header).typeflag = BLKTYPE;
            break;
    }

    _strcpy((*tar_header).magic, TMAGIC);
    _strcpy((*tar_header).uname, pwd->pw_name);
    _strcpy((*tar_header).gname, grp->gr_name);
    _strcpy((*tar_header).version, TVERSION);

    
    unsigned int chk = 0;
    chk = checksum((*tar_header).name, HEADER_NAME_SIZE, chk);
    chk = checksum((*tar_header).mode, HEADER_MODE_SIZE, chk);
    chk = checksum((*tar_header).uid, HEADER_UID_SIZE, chk);
    chk = checksum((*tar_header).gid, HEADER_GID_SIZE, chk);
    chk = checksum((*tar_header).size, HEADER_SIZE_SIZE, chk);
    chk = checksum((*tar_header).mtime, HEADER_MTIME_SIZE, chk);
    chk += (*tar_header).typeflag;
    chk += 8*32;
    chk = checksum((*tar_header).linkname, HEADER_LINKNAME_SIZE, chk);
    chk = checksum((*tar_header).magic, HEADER_MAGIC_SIZE, chk);
    chk = checksum((*tar_header).version, HEADER_VERSION_SIZE, chk);
    chk = checksum((*tar_header).uname, HEADER_UNAME_SIZE, chk);
    chk = checksum((*tar_header).gname, HEADER_GNAME_SIZE, chk);
    chk = checksum((*tar_header).prefix, HEADER_PREFIX_SIZE, chk);

    copyOctal((*tar_header).chksum, chk, (HEADER_CHKSUM_SIZE - 1));
    (*tar_header).chksum[HEADER_CHKSUM_SIZE - 1] = BLANK;
}

int process_header(int archive_fd, char* archive_member, struct stat archive_member_info)
{

    struct posix_header *tar_header = malloc(sizeof(struct posix_header));
    memset(tar_header, 0, sizeof(struct posix_header));

    fill_header(archive_member, tar_header, archive_member_info);
    write(archive_fd, tar_header, sizeof(struct posix_header));
    free(tar_header);

    return 0;
}