#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libgen.h>
#include <ctype.h>
#include <dirent.h>

static  const  char *dirpath = "/home/alecetra/Downloads";
static const int buffer_size = 1024;

char *get_dir_name(char *path){
    char *filename = malloc(sizeof(char) * buffer_size);
    int n = strlen(path);
    int i = n - 1;
    while (i >= 0){
        if (path[i] == '/') break;
        i--;
    }
    i++;
    snprintf(filename, i, "%s", path);
    return filename;
}

char *get_file_name(char *path){
    char *filename = malloc(sizeof(char) * buffer_size);
    int n = strlen(path);
    int i = n - 1;
    while (i >= 0){
        if (path[i] == '/') break;
        i--;
    }
    i++;
    sprintf(filename, "%s", path + i);
    return filename;
}

char *get_file_name_only(char *path){
    char *filename = malloc(sizeof(char) * buffer_size);
    char *temppath = get_file_name(path);
    int n = strlen(temppath);
    int i = 0;
    for (;i < n;i++){
        if (temppath[i] == '.'){
            break;
        }
    }
    snprintf(filename, i, "%s", temppath);
    free(temppath);
    return filename;
}

char *get_extension_name(char *path){
    char *filename = malloc(sizeof(char) * buffer_size);
    char *temppath = get_file_name(path);
    int n = strlen(temppath);
    int i = 0;
    for (;i < n;i++){
        if (temppath[i] == '.'){
            break;
        }
    }
    i++;
    sprintf(filename, "%s", temppath + i);
    free(temppath);
    return filename;
}

int get_lowercase_diff_decimal(char *path){
    int val = 0;

    int n = strlen(path);
    int k = n;

    int i = 0;

    for (;i < k;i++){
        int diff = (path[i] == tolower(path[i]) ? 0 : 1);
        val <<= 1;
        val |= diff;
    }

    return val;
}

char *get_special_directory_name(char *path){
    char *filename = malloc(sizeof(char) * buffer_size);
    char *filename_only = get_file_name_only(path);
    char *extension_only = get_extension_name(path);

    //printf("%s\n", filename_only);
    //printf("%s\n", extension_only);

    int diff = get_lowercase_diff_decimal(filename_only);

    int i = 0;
    int n = strlen(filename_only);
    for (;i < n;i++){
        filename_only[i] = tolower(filename_only[i]);
    }

    sprintf(filename, "%s.%s.%d", filename_only, extension_only, diff);

    free(filename_only);
    free(extension_only);

    return filename;
}

bool check_is_special_directory(char *path){
    return strstr(path, "A_is_a_") == path;
}

void change_to_special_directory(char *path){
    if (check_is_special_directory(path))
    {

    } else {

    }
}

char *encrypt_atbash (char* str){
    int i=0;
    if(!strcmp(str,".") || !strcmp(str,"..")) return str;
    
    char *output = malloc(buffer_size);

    while(str[i]!='\0')
    {
        if(str[i]=='.')
        {
            break;
        }
        if(!((str[i]>=0&&str[i]<65)||(str[i]>90&&str[i]<97)||(str[i]>122&&str[i]<=127)))
        {
            if(str[i]>='A'&&str[i]<='Z')
                output[i] = 'Z'+'A'-str[i];
            if(str[i]>='a'&&str[i]<='z')
                output[i] = 'z'+'a'-str[i];
        } else {
		output[i] = str[i];
	}
            
        i++;
    }

    while (str[i] != '\0'){
	    output[i] = str[i];
    	i++;
    }

    output[i] = '\0';

    return output;
}

int recursively_encode_atoz(char *fpath){
    DIR *dir;
    struct dirent *dp;

    dir = opendir(fpath);

    if (dir == NULL) return -errno;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
		    if(dp->d_type == DT_REG) {
                char path1[buffer_size];

                char old_name[buffer_size];
                char *dir_name = fpath;
                char *file_name = get_file_name(dp->d_name);

                sprintf(old_name, "%s/%s", dir_name, dp->d_name);
                sprintf(path1, "%s/%s", dir_name, encrypt_atbash(file_name));
                
                rename(old_name, path1);

                printf("Rename from : %s to %s\n", old_name, path1);
            } else if(dp->d_type == DT_DIR) {
                char path2[buffer_size];

                char old_name[buffer_size];
                char *dir_name = fpath;
                char *file_name = get_file_name(dp->d_name);

                sprintf(old_name, "%s/%s", dir_name, dp->d_name);
                sprintf(path2, "%s/%s", dir_name, encrypt_atbash(file_name));

                rename(old_name, path2);

                printf("Rename from : %s to %s\n", old_name, path2);

                recursively_encode_atoz(path2);
            }
	     }
    }

    closedir(dir);
    return 0;
}

void recursively_check_encoding(char *from, char* to){
    char *toName = get_file_name(to);
    char *fromName = get_file_name(from);

    printf("REC : %s -> %s\n", fromName, toName);
    if(strstr(fromName, "AtoZ_") == NULL && strstr(toName, "AtoZ_") == toName){
        //encode atoz
        printf("Initiate encode AtoZ\n");
        recursively_encode_atoz(to);
    } else if (strstr(fromName, "AtoZ_") == fromName && strstr(toName, "AtoZ_") == NULL){
        //decode atoz
        printf("Initiate decode AtoZ\n");
        recursively_encode_atoz(to);
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);

    printf("GetAttr : %s\n", fpath);

	int res;

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("ReadDir : %s\n", fpath);

    int res = 0;

    DIR *dp;
    struct dirent *de;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));
        char temp[buffer_size];
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        strcpy(temp, de->d_name);
        res = (filler(buf, temp, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("ReadFile : %s\n", fpath);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int xmp_mkdir(const char * path, mode_t mode){
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("MkDir : %s\n", fpath);

    int res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    char fpath[buffer_size];
    char tpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, from);
    sprintf(tpath, "%s%s", dirpath, to);

    printf("RenameFrom : %s\n", fpath);
    printf("RenameTo : %s\n", tpath);

	int res;

	res = rename(fpath, tpath);

    recursively_check_encoding(fpath, tpath);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("Unlink : %s\n", fpath);

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path){
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("RmDir : %s\n", fpath);
    
    int res = rmdir(fpath);
    if(res == -1){
        return -errno;
    } else {
        printf("penulisan dan rmdir sukses\n");
    } 
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];    

    sprintf(fpath, "%s%s", dirpath, path);

    printf("Write : %s\n", fpath);

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char fpath[buffer_size];    

    sprintf(fpath, "%s%s", dirpath, path);

    printf("MakeNod : %s\n", fpath);

	int res;

	/* On Linux this could just be 'mknod(fpath, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    char fpath[buffer_size];    

    sprintf(fpath, "%s%s", dirpath, path);

    printf("Open : %s\n", fpath);

	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .mkdir = xmp_mkdir,
    .mknod = xmp_mknod,
    .rename = xmp_rename,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write
};

int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
