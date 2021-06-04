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

char *get_file_name(char *path){
    char *filename = malloc(sizeof(char) * 256);
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
    char *filename = malloc(sizeof(char) * 256);
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
    char *filename = malloc(sizeof(char) * 256);
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
    char *filename = malloc(sizeof(char) * 256);
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
    
    char *output = malloc(1024);

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

    return output;
}

static  int  xmp_getattr(const char *path, struct stat *st)
{

    char fpath[2056];
    char fnpath[2056];

    sprintf(fpath, "%s%s", dirpath, path);

    printf("GetAttr : %s\n", fpath);

    char *filename = get_file_name(fpath);
    char *encryptedfilename = filename;

    if(strstr(filename, "AtoZ_") == filename)
    {
        encryptedfilename = encrypt_atbash(filename);
    } else {
	
    }

    sprintf(fnpath, "%s/%s", dirpath, encryptedfilename);

    printf("GetAttrAtbash : %s\n", fnpath);

    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time( NULL );
    st->st_mtime = time( NULL );

    if ( strcmp( path, "/" ) == 0 ) {
	    	st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2;
    } else {
	    	st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
    }

    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    //char name[1000];
    int mode=0;
    if(strstr(path, "AtoZ_")!=NULL)
    {
        mode=1;
    }

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else {
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res = 0;

    DIR *dp;
    struct dirent *de;
    // (void) offset;
    // (void) fi;
    // char paths[100];
   
    // strcpy(paths, path);

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));
        char temp[1000];
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        strcpy(temp, de->d_name);
        if(mode==1)
        {
            encrypt_atbash(temp);
        }
        res = (filler(buf, temp, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else {
        char temp[1000];
        strcpy(temp, path);

        if(strstr(path, "/AtoZ_")!=NULL)
            encrypt_atbash(temp);

        sprintf(fpath, "%s%s",dirpath,temp);
    }

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

static int xmp_rmdir(const char *path){
    //printf("Enter => rmdir\n");
    int res;
    char fpath[1000] = "/home/alecetra/Downloads";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    res = rmdir(fpath);
    if(res == -1){
        return -errno;
    }else{
        printf("penulisan dan rmdir sukses\n");
    }        
    return 0;
}


static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .rmdir = xmp_rmdir,
    .read = xmp_read,
};



int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
