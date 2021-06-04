#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/alecetra/Downloads";

void encrypt (char* str){
    int i=0;
    if(!strcmp(str,".") || !strcmp(str,"..")) return;
 int panjang = strlen(str);
    //int start=0;
    for(int i=panjang;i>=0;i--)
 {
  if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
 }
    for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/')
        {
            //start = i;
            break;
        }
      
    }

    while(str[i]!='\0')
    {
        if(str[i]=='.')
        {
            break;
        }
        if(!((str[i]>=0&&str[i]<65)||(str[i]>90&&str[i]<97)||(str[i]>122&&str[i]<=127)))
        {
            if(str[i]>='A'&&str[i]<='Z')
                str[i] = 'Z'+'A'-str[i];
            //printf("%c",'Z'+'A'-str[i]);
            if(str[i]>='a'&&str[i]<='z')
                str[i] = 'z'+'a'-str[i];
            //printf("%c",'z'+'a'-str[i]);
        } 
            
        if(((str[i]>=0&&str[i]<65)||(str[i]>90&&str[i]<97)||(str[i]>122&&str[i]<=127)))
        {
            //printf("%c",str[i]);    
        }
            
        i++;
    }
  //printf("\n");
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    char name[1000];
    strcpy(name,path);
    if(strstr(path, "AtoZ_")!=NULL)
    {
        encrypt(name);
    }
    else
    {
        strcpy(name,path);
    }
    sprintf(fpath,"%s%s",dirpath,name);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

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
            encrypt(temp);
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
            encrypt(temp);

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
