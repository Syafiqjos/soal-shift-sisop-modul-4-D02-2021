#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libgen.h>
#include <ctype.h>
#include <dirent.h>

//dasar, ngambil filename.txt
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

//dasar, ngambil filename aja
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

//dasar, ngambil .txt aja
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

//buat nomer 3
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

//buat nomer 3
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

//buat nomer 3, check kalo dia special directory
bool check_is_special_directory(char *path){
    return strstr(path, "A_is_a_") == path;
}

//buat nomer 3, panggil ketika rename
void change_to_special_directory(char *path){
    if (check_is_special_directory(path)){
        //ubah ke spesial directory
    } else {
        //ubah ke normal directory
    }
}

int main(){
    if (check_is_special_directory("A_is_a_somemf")){
        printf("Yeah\n");
    } else {
        printf("Nope\n");
    }

    printf("%s\n", get_special_directory_name("some/thing/is/NOThing.txt.gta"));

    system("pause");
}