#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <sys/sysmacros.h>
#include <dirent.h>

#define MODE_C "-cf"
#define MODE_X "-xf"
#define MODE_R "-rf"
#define MODE_U "-uf"
#define MODE_T "-tf"

// auxilary.c
int _strlen(char *string);
void _strcpy(char *destinantion, char *source);
int _strcmp(char *string1, char *string2);
char* _strcat(char* string1, char* string2);
unsigned int octal2uint(char *octal, unsigned int size);

// mode_u.c
//int compare_names(struct posix_header *file_header, int archive_fd, char *new_archive_member);
//int read_content(int archive_fd, int remaining_content, char *new_archive_member);

int mode_u(int count, char** input);

// mode_t.c
int mode_ls(char** input);

// mode_r.c
int mode_r(int count, char** input);

// mode_x.c
//int fill_file_info(struct stat *archive_member_info, struct posix_header *file_header);
//int process_file_header(struct posix_header *file_header, int archive_fd);
int ifexists(char *path);
int extract(int archive_fd, int remaining_content);
int mode_x(char** input);

// mode_c.c
int mode_c(int count, char**  input);
int process_archive_member(int archive_fd, char* archive_member);
int write_content(int archive_fd, char* archive_member);
int write_end_of_file(int archive_fd, int file_size);

struct Content
{
    int size;
    char* list[50];
};


// xheader.c

struct posix_header
{                              /* byte offset */
    char name[100];               /*   0 */
    char mode[8];                 /* 100 */
    char uid[8];                  /* 108 */
    char gid[8];                  /* 116 */
    char size[12];                /* 124 */
    char mtime[12];               /* 136 */
    char chksum[8];               /* 148 */
    char typeflag;                /* 156 */
    char linkname[100];           /* 157 */
    char magic[6];                /* 257 */
    char version[2];              /* 263 */
    char uname[32];               /* 265 */
    char gname[32];               /* 297 */
    char devmajor[8];             /* 329 */
    char devminor[8];             /* 337 */
    char prefix[155];             /* 345 */
    char block[12];               /* 500 */
                                  /* 512 */
};

#define TMAGIC   "ustar "        /* ustar and a null */
#define TVERSION " "           /* 00 and no null */

/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

#define XHDTYPE  'x'            /* Extended header referring to the
                                   next file in the archive */
#define XGLTYPE  'g'            /* Global extended header */

/* Bits used in the mode field, values in octal.  */
#define TSUID    04000          /* set UID on execution */
#define TSGID    02000          /* set GID on execution */
#define TSVTX    01000          /* reserved */
                                /* file permissions */
#define TUREAD   00400          /* read by owner */
#define TUWRITE  00200          /* write by owner */
#define TUEXEC   00100          /* execute/search by owner */
#define TGREAD   00040          /* read by group */
#define TGWRITE  00020          /* write by group */
#define TGEXEC   00010          /* execute/search by group */
#define TOREAD   00004          /* read by other */
#define TOWRITE  00002          /* write by other */
#define TOEXEC   00001          /* execute/search by other */

#define BLOCK 512
#define BLANK ' '

#define HEADER_NAME_SIZE 100
#define HEADER_MODE_SIZE 8
#define HEADER_UID_SIZE 8
#define HEADER_GID_SIZE 8
#define HEADER_SIZE_SIZE 12
#define HEADER_MTIME_SIZE 12
#define HEADER_CHKSUM_SIZE 8
#define HEADER_LINKNAME_SIZE 100
#define HEADER_MAGIC_SIZE 6
#define HEADER_VERSION_SIZE 2
#define HEADER_UNAME_SIZE 32
#define HEADER_GNAME_SIZE 32
#define HEADER_DEVMAJOR_SIZE 8
#define HEADER_DEVMINOR_SIZE 8
#define HEADER_PREFIX_SIZE 155

int process_header(int archive_fd, char* archive_member, struct stat archive_member_info);


void copyOctal(char *dest, unsigned int num, unsigned char size);
void fill_header(char* archive_member, struct posix_header *tar_header, struct stat archive_member_info);
unsigned int checksum (char ptr[], int sz, unsigned int chk);

#endif