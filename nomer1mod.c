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
static const char *log_path = "/home/alecetra/SinSeiFS.log";
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
    snprintf(filename, i + 1, "%s", temppath);
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


int log_info_command(char *command, char *from, char *to){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
    if(to == NULL){
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
	printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}

int log_warning_command(char *command, char *from, char *to){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
    if(to == NULL){
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
	printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}

bool check_is_special_directory(char *path){
    return strstr(path, "A_is_a_") == path;
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

char *encrypt_rot13(char *str)
{
    int i =0;
    if(str == NULL){
      return NULL;
    }
    if(!strcmp(str,".") || !strcmp(str,"..")) return str;
    
    char* result = malloc(strlen(str));
    strcpy(result, str);
    if(result != NULL){      
        while(str[i] != '\0'){
            if(str[i] =='.')
            {
                break;
            }
            //Only increment alphabet characters
            if((str[i] >= 97 && str[i] <= 122) || (str[i] >= 65 && str[i] <= 90)){
                if(str[i] > 109 || (str[i] > 77 && str[i] < 91)){
                    //Characters that wrap around to the start of the alphabet
                    result[i] -= 13;
                }else{
                    //Characters that can be safely incremented
                    result[i] += 13;
                }
            }
            i++;
        }
        while (str[i] != '\0'){
            result[i] = str[i];
            i++;
        }
    }
    result[i] = '\0';
    return result;
}

//ini masukin ke variable global aja
char KEY [] = "SISOP";

char *encrypt_vignere(char *str){
    int i,j;
    int msgLen = strlen(str);
    int keyLen = strlen(KEY);
    char newKey[msgLen];
    char *encryptedMsg = malloc(msgLen);

    if(!strcmp(str,".") || !strcmp(str,"..")) return str;
    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
        if(j == keyLen)
            j = 0;
 
        newKey[i] = KEY[j];
    }
 
    newKey[i] = '\0';

    //encryption
    for(i = 0; i < msgLen; ++i){
        if(str[i]=='.')
        {
            break;
        }
        encryptedMsg[i] = ((str[i] + newKey[i]) % 26) + 'A';
    }

    while (str[i] != '\0'){
     encryptedMsg[i] = str[i];
     i++;
    }

    encryptedMsg[i] = '\0';

    return encryptedMsg;

}

char *decrypt_vignere(char *str){
    int i,j;
    int msgLen = strlen(str);
    int keyLen = strlen(KEY);
    char newKey[msgLen];
    char *decryptedMsg= malloc(msgLen); 

    if(!strcmp(str,".") || !strcmp(str,"..")) return str;
    
    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
        if(j == keyLen)
            j = 0;
 
        newKey[i] = KEY[j];
    }
 
    newKey[i] = '\0';

    //decryption
    for(i = 0; i < msgLen; ++i){
        if(str[i]=='.')
        {
            break;
        }
        decryptedMsg[i] = (((str[i] - newKey[i]) + 26) % 26) + 'A';
    }

    while (str[i] != '\0'){
     decryptedMsg[i] = str[i];
     i++;
    }
    decryptedMsg[i] = '\0';

    return decryptedMsg;

}

// int recursively_encode_atoz(char *fpath){
//     DIR *dir;
//     struct dirent *dp;

//     dir = opendir(fpath);

//     if (dir == NULL) return -errno;

//     while ((dp = readdir(dir)) != NULL) {
//         if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
// 		    if(dp->d_type == DT_REG) {
//                 char path1[buffer_size];

//                 char old_name[buffer_size];
//                 char *dir_name = fpath;
//                 char *file_name = get_file_name(dp->d_name);

//                 sprintf(old_name, "%s/%s", dir_name, dp->d_name);
//                 sprintf(path1, "%s/%s", dir_name, encrypt_atbash(file_name));
                
//                 rename(old_name, path1);

//                 printf("Rename from : %s to %s\n", old_name, path1);
//                 log_info_command("RENAME", old_name, path1);
//             } else if(dp->d_type == DT_DIR) {
//                 char path2[buffer_size];

//                 char old_name[buffer_size];
//                 char *dir_name = fpath;
//                 char *file_name = get_file_name(dp->d_name);

//                 sprintf(old_name, "%s/%s", dir_name, dp->d_name);
//                 sprintf(path2, "%s/%s", dir_name, encrypt_atbash(file_name));

//                 rename(old_name, path2);

//                 printf("Rename from : %s to %s\n", old_name, path2);
//                 log_info_command("RENAME", old_name, path2);
//                 recursively_encode_atoz(path2);
//             }
// 	     }
//     }

//     closedir(dir);
//     return 0;
// }

// void recursively_check_encoding(char *from, char* to){
//     char *toName = get_file_name(to);
//     char *fromName = get_file_name(from);

//     printf("REC : %s -> %s\n", fromName, toName);
//     if(strstr(fromName, "AtoZ_") == NULL && strstr(toName, "AtoZ_") == toName){
//         //encode atoz
//         printf("Initiate encode AtoZ\n");
//         recursively_encode_atoz(to);
//     } else if (strstr(fromName, "AtoZ_") == fromName && strstr(toName, "AtoZ_") == NULL){
//         //decode atoz
//         printf("Initiate decode AtoZ\n");
//         recursively_encode_atoz(to);
//     }
// }

// 0 == none, 1 == rx 1, 2 == rx 2
int get_rx_mode(char *path){
    char fpath[buffer_size];
    sprintf(fpath, "%s/%s", path, ".status_rx_1");
    if (access(fpath, F_OK) == 0){
        return 1;
    }

    sprintf(fpath, "%s/%s", path, ".status_rx_2");
    if (access(fpath, F_OK) == 0){
        return 2;
    }

    return 0;
}

//mode << 0 == none,1 << 0 = atoz,1 << 1 = rx_1, << 2 = rx_2, << 3 = special
int get_encryption_mode(char *path, int mode, bool force_return){
    if (strlen(path) == 0){
        return mode;
    }
    char *temp = path;
    char *fname = get_file_name(temp);

    //printf("REC : %s -> %d\n", fname, mode);

    if (strstr(fname, "A_is_a_") == fname){
        mode |= 1 << 3;
        return mode;
    }
    else if (strstr(fname, "AtoZ_") == fname){
        mode |= 1 << 0;
        if (force_return) return mode;
    } else if (strstr(fname, "RX_") == fname && get_rx_mode(path) == 1){
        mode |= 1 << 1;
        if (force_return) return mode;
    } else if (strstr(fname, "RX_") == fname && get_rx_mode(path) == 2){
        mode |= 1 << 2;
        if (force_return) return mode;
    }

    temp = get_dir_name(path);

    int moden = get_encryption_mode(temp, mode, force_return);

    free(temp);
    return moden;
}

char *get_encryption_path(const char * path, bool force_return){
    char *fpath = malloc(buffer_size);
    char temp[buffer_size];
    
    sprintf(temp, "%s", path);
    int enc_mode = get_encryption_mode(get_dir_name(temp), 0, force_return);

    if(enc_mode == 0){
        sprintf(fpath, "%s%s", dirpath, path);
    } else if ((enc_mode & 1)){
        //sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(temp), encrypt_atbash(get_file_name(temp)));
        sprintf(fpath, "%s", dirpath);
        char *test = strtok(temp, "/");
        bool trig = false;
        while (test){
            int n = strlen(fpath);
            if (trig){
                sprintf(fpath + n, "/%s", encrypt_atbash(test));
            } else {
                sprintf(fpath + n, "/%s", test);
            }
            if (strstr(test, "AtoZ_") == test){
                trig = true;
            }
            test = strtok(NULL, "/");
        }
    } else if (enc_mode == 2){
        //sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(temp), encrypt_atbash(get_file_name(temp)));
    } else if (enc_mode == 3){
        //sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(temp), encrypt_atbash(get_file_name(temp)));
    } else if (enc_mode == 4){
        //sprintf(fpath, "%s%s", dirpath, path);
        //sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(temp), get_special_directory_name(temp));
    }

    return fpath;
}

bool force_sebenarnya = false;

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char *fpath;

    if (force_sebenarnya){
        fpath = malloc(buffer_size);
        sprintf(fpath, "%s%s", dirpath, path);
    } else {
        fpath = get_encryption_path(path, false);
    }

    printf("GetAttr : %s\n", fpath);

	int res;

    force_sebenarnya = false;

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    //char *fpath = get_encryption_path(path, false);
    char fpath[buffer_size];

    sprintf(fpath, "%s%s", dirpath, path);

    int enc_mode = get_encryption_mode(fpath, 0, false);

    if (enc_mode == 0){
        
    } else if ((enc_mode & 1)){
        sprintf(fpath, "%s", get_encryption_path(path, false));
    }

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

        if(enc_mode == 0){
            sprintf(temp, "%s", de->d_name);
        } else if ((enc_mode & 1)){
            //sprintf(temp, "%s", encrypt_atbash(de->d_name));
            //printf("de -> %s\n", de->d_name);
            sprintf(temp, "%s", encrypt_atbash(de->d_name));
        } else if (enc_mode == 2){
            //sprintf(temp, "%s", encrypt_atbash(de->d_name));
        } else if (enc_mode == 3){
            //sprintf(temp, "%s", encrypt_atbash(de->d_name));
        } else if (enc_mode == 4){
            //sprintf(temp, "%s", get_special_directory_name(de->d_name));
        }

        printf("?? -> %s\n", temp);

        res = (filler(buf, temp, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char *fpath = get_encryption_path(path, false);

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
    char temp[buffer_size];
    sprintf(temp, "%s", path);
    char *temp2 = get_encryption_path(get_dir_name(temp), false);
    char fpath[buffer_size];
    sprintf(fpath, "%s/%s", temp2, get_file_name(temp));

    printf("MkDir : %s\n", fpath);

    force_sebenarnya = true;

    int res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    char *fpath = get_encryption_path(from, false);
    char *tpath = get_encryption_path(to, false);

    //sprintf(fpath, "%s%s", dirpath, from);
    //sprintf(tpath, "%s%s", dirpath, to);

    printf("RenameFrom : %s\n", fpath);
    printf("RenameTo : %s\n", tpath);

	int res;

	res = rename(fpath, tpath);

    //recursively_check_encoding(fpath, tpath);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
    char *fpath = get_encryption_path(path, false);
    
    printf("Unlink : %s\n", fpath);

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path){
    char *fpath = get_encryption_path(path, false);

    printf("RmDir : %s\n", fpath);
    
    int res = rmdir(fpath);
    if(res == -1){
        return -errno;
    }
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char *fpath = get_encryption_path(path, false);

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
    char temp[buffer_size];
    sprintf(temp, "%s", path);
    char *temp2 = get_encryption_path(get_dir_name(temp), false);
    char fpath[buffer_size];
    sprintf(fpath, "%s/%s", temp2, get_file_name(temp));

    printf("MakeNod : %s\n", fpath);

    force_sebenarnya = true;

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
    char *fpath = get_encryption_path(path, false);

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
