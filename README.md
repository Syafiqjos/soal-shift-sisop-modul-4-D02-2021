# soal-shift-sisop-modul-4-D02-2021
- Jessica Tasyanita (05111940000043)
- Ahmad Syafiq Aqil Wafi (05111940000089)
- Maximilian H M Lingga (05111940000092)

## Soal 1 (AtoZ_folder)

### Tujuan

Membuat sistem FUSE pada path `/home/[USER]/Downloads` yang dapat digunakan untuk encode dan decode. Ketika suatu directory memiliki substring "AtoZ_" pada awal nama directory-nya maka directory tersebut akan di encode menggunakan Atbash Cypher. Namun ketika directory tersebut di rename menjadi nama yang tidak diawali dengan "AtoZ_" maka directory tersebut akan di decode. Terdapat ketentuan juga bahwa encode an yang terjadi berjalan secara recursive, jadi file atau folder yang di dalam directory tersebut akan ikut ter-encode.

### Pengerjaan

Pertama adalah melakukan inisialisasi sebagai path yang akan di encode / decode. Dalam kasus ini adalah path `/home/[USER]/Downloads`. `buffer_size` merupakan batas size dari string pada saat pembuatan string.

```C
static  const  char *dirpath = "/home/alecetra/Downloads";
static const char *log_path = "/home/alecetra/SinSeiFS.log";
static const int buffer_size = 1024;
```

Pada dasarnya kami melakukan check pada suatu path. Path akan diperiksa bahwa path ini akan menggunakan enkripsi atau tidak, jika menggunakan enkripsi maka jenis enkripsi apa yang digunakan. Dalam kasus ini kami membuat suatu fungsi yang akan menentukan jenis enkripsi-nya.

```C
//mode << 0 == none,1 << 0 = atoz,1 << 1 = rx_1, << 2 = rx_2, << 3 = special
int get_encryption_mode(char *path){
    if (strlen(path) == 0){
        return 0;
    }
    char *temp = path;

    int mode = 0;

    //printf("REC : %s -> %d\n", fname, mode);
    char *tok = strtok(temp, "/");
    char fpath[buffer_size];
    fpath[0] = 0;

    sprintf(fpath, "%s", dirpath);

    while (tok){
        sprintf(fpath + strlen(fpath), "/%s", tok);

        int rx_mode = get_rx_mode(fpath);

        printf("kok ngebug : %d\n", rx_mode);

        if (strstr(tok, "A_is_a_") == tok){
            mode |= 1 << 3;
            return mode;
        }
        else if (strstr(tok, "AtoZ_") == tok){
            mode |= 1 << 0;
        } else if (strstr(tok, "RX_") == tok && rx_mode == 1){
            mode |= 1 << 1;
        } else if (strstr(tok, "RX_") == tok && rx_mode == 2){
            mode |= 1 << 2;
        }
        tok = strtok(NULL, "/");
    }

    return mode;
}
```

Fungsi diatas akan memeriksa jenis enkripsi apa yang akan digunakan. Kemudian fungsi berikut akan membentuk suatu path baru / path yang di enkripsi sesuai dengan jenis enkripsi yang didapatkan dari fungsi diatas.

```C
char *get_encryption_path(const char * path){
    if (strlen(path) == 0 || path[0] == 0){
        return "/";
    }

    char *fpath = malloc(buffer_size);
    char temp[buffer_size];
    
    fpath[0] = 0;

    sprintf(temp, "%s", path);

    //printf("initial : %s\n", temp);

    char *test = temp;
    char *mid = temp;

    int enc = 0;

    while (*mid != '\0'){
        int n = strlen(fpath);

        mid = test;
        while ((*mid) != '/' && (*mid) != '\0'){
            mid++;
        }

        char jawaban[buffer_size];
        snprintf(jawaban, mid - test + 1, "%s", test);

        if (enc == 0){
            sprintf(fpath + n, "/%s", jawaban);
        } else if (enc & (1 << 0)){
            sprintf(fpath + n, "/%s", encrypt_atbash(jawaban));
        } else if (enc & (1 << 1)){
            sprintf(fpath + n, "/%s", encrypt_rot13(encrypt_atbash(jawaban)));
        } else if (enc & (1 << 2)){
            sprintf(fpath + n, "/%s", encrypt_vignere(encrypt_atbash(jawaban)));
        } else if (enc & (1 << 3)){
            //sprintf(fpath + n, "/%s", jawaban);
            sprintf(fpath + n, "/%s", get_special_directory_name(jawaban));
        }

        int enc_temp = get_encryption_mode(jawaban);
        if (!(enc & (1 << 3))){
            enc = enc_temp;
        }

        test = mid + 1;
    }

    //printf("end : %s\n", fpath);

    return fpath + 1;
}
```

Kemudian ada juga fungsi yang kami gunakan untuk melakukan decryption / decode. Sebenarnya mirip dengan fungsi diatas, perbedaannya adalah bagian decryption vignere (Nomor 2) dan directory special (Nomor 3).

```C
char *get_decryption_path(const char * path){
    if (strlen(path) == 0 || path[0] == 0){
        return "/";
    }

    char *fpath = malloc(buffer_size);
    char temp[buffer_size];
    
    fpath[0] = 0;

    sprintf(temp, "%s", path);

    //printf("initial : %s\n", temp);

    char *test = temp;
    char *mid = temp;

    int enc = 0;

    while (*mid != '\0'){
        int n = strlen(fpath);

        mid = test;
        while ((*mid) != '/' && (*mid) != '\0'){
            mid++;
        }

        char jawaban[buffer_size];
        snprintf(jawaban, mid - test + 1, "%s", test);

        if (enc == 0){
            sprintf(fpath + n, "/%s", jawaban);
        } else if (enc & (1 << 0)){
            sprintf(fpath + n, "/%s", encrypt_atbash(jawaban));
        } else if (enc & (1 << 1)){
            sprintf(fpath + n, "/%s", encrypt_atbash(encrypt_rot13(jawaban)));
        } else if (enc & (1 << 2)){
            sprintf(fpath + n, "/%s", encrypt_atbash(decrypt_vignere(jawaban)));
        } else if (enc & (1 << 3)){
            //sprintf(fpath + n, "/%s", jawaban);
            sprintf(fpath + n, "/%s", get_special_directory_original_name(jawaban));
        }

        // printf("decprocess : %s -> %d\n", fpath, enc);

        int enc_temp = get_encryption_mode(jawaban);
        if (!(enc & (1 << 3))){
        // if (enc == 0){
            enc = enc_temp;
        }

        test = mid + 1;
    }

    //printf("end : %s\n", fpath);

    return fpath + 1;
}
```

Untuk nomer 1 sendiri, berikut merupakan fungsi untuk melakukan enkripsi menggunakan algoritma Atbash.

--- PENJELASAN ATBASH ---

```C
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
```

Selanjutnya adalah proses enkripsi dan dekripsi yang berjalan pada system FUSE kami. Pada dasarnya kami melakukan enkripsi pada bagian FUSE `readdir` dan melakukan dekripsi pada bagian selain `readdir` seperti `read`, `write`, `getattr`, `mkdir` dan lain sebagainya.

Berikut merupakan fungsi `get_attr` yang kami gunakan.

```C
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));

    printf("GetAttr : %s\n", fpath);

	int res;

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}
```

Dapat dilihat bahwa fungsi melakukan dekripsi dan digabung dengan path yang akan di FUSE, kami menggunakan `dirpath` yang memiliki isi string `home/alecetra/Downloads`.

Untuk readdir sendiri kita akan lakukan encode agar pada terminal console akan tampak directory yang di encode. Berikut merupakan fungsi `readdir`.

```C
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];
    char path_non_constant[buffer_size];

    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));
    sprintf(path_non_constant, "%s", path);

    printf("ReadDir : %s\n", fpath);

    int res = 0;

    int enc = get_encryption_mode(path_non_constant);

    DIR *dp;
    struct dirent *de;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    //log_info_command("READDIR", path, NULL);

    while ((de = readdir(dp)) != NULL) {
        if (de->d_name[0] == '.') continue;
        struct stat st;

        char temp[buffer_size];

        if (enc == 0){
            sprintf(temp, "/%s", de->d_name);
        } else if (enc & (1 << 0)){
            sprintf(temp, "/%s", encrypt_atbash(de->d_name));
        } else if (enc & (1 << 1)){
            sprintf(temp, "/%s", encrypt_rot13(encrypt_atbash(de->d_name)));
        } else if (enc & (1 << 2)){
            sprintf(temp, "/%s", encrypt_vignere(encrypt_atbash(de->d_name)));
        } else if (enc & (1 << 3)){
            // sprintf(temp, "/%s", de->d_name);
            sprintf(temp, "/%s", get_special_directory_name(de->d_name));
        }

        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        printf("to be filler : %d -> %s\n", enc, de->d_name);
        printf("?? -> %s\n", temp);

        res = (filler(buf, temp + 1, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}
```

Dapat dilihat bahwa macam - macam enkripsi dilakukan pada fungsi ini. Lalu kita gunakan `filler` untuk mengganti nama directory, sehingga pada fungsi `getattr` sebelumnya harus di-decode untuk menghindari error `path not found` yang dapat terjadi.

Sama halnya dengan fungsi `get_attr` fungsi selain `readdir` juga menggunakan dekripsi dalam implementasinya.

```C
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));

    printf("ReadFile : %s\n", fpath);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    log_info_command("READ", path, NULL);

    return res;
}
```

```C
static int xmp_mkdir(const char * path, mode_t mode){
    char castpath[buffer_size];
    sprintf(castpath, "%s", path);
    
    char fpath[buffer_size];

    //sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));
    sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(get_decryption_path(castpath)), get_file_name(castpath));

    printf("MkDir : %s\n", fpath);

    int res = mkdir(fpath, mode);

	if (res == -1)
		return -errno;

    char temp[buffer_size];
    sprintf(temp, "%s", get_file_name(fpath));
    if (strstr(temp, "RX_") == temp){
        sprintf(temp, "%s/%s", fpath, ".status_rx_1");
        FILE *file = fopen(temp, "w");
        fclose(file);
    }

    log_info_command("MKDIR", path, NULL);

	return 0;
}
```

```C
static int xmp_rename(const char *from, const char *to)
{
    char castpath[buffer_size];
    sprintf(castpath, "%s", to);
    
    char fpath[buffer_size];
    char tpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(from));
    sprintf(tpath, "%s%s/%s", dirpath, get_dir_name(get_decryption_path(castpath)), get_file_name(castpath));

    //sprintf(fpath, "%s%s", dirpath, from);
    //sprintf(tpath, "%s%s", dirpath, to);

    printf("RenameFrom : %s\n", fpath);
    printf("RenameTo : %s\n", tpath);

	int res;

	res = rename(fpath, tpath);

	if (res == -1)
		return -errno;

    //rx status
    char temp_rz1[buffer_size];
    char temp[buffer_size];
    sprintf(temp, "%s", get_file_name(tpath));
    sprintf(temp_rz1, "%s/%s", tpath, ".status_rx_1");

    //if status rz1 exists
    if (access(temp_rz1, F_OK) == 0){
        if (strstr(temp, "RX_") == temp){
            unlink(temp_rz1);
            sprintf(temp, "%s/%s", tpath, ".status_rx_2");
            FILE *file = fopen(temp, "w");
            fclose(file);
        }
    }

    log_info_command("RENAME", from, to);

	return 0;
}
```

```C
static int xmp_unlink(const char *path)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));
    
    printf("Unlink : %s\n", fpath);

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

    log_warning_command("UNLINK", path, NULL);

	return 0;
}
```

```C
static int xmp_rmdir(const char *path){
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));

    printf("RmDir : %s\n", fpath);
    
    int res = rmdir(fpath);
    if(res == -1){
        return -errno;
    }

    log_warning_command("RMDIR", path, NULL);

    return 0;
}
```

```C
static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));

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

    log_info_command("WRITE", path, NULL);

	return res;
}
```

```C
static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char castpath[buffer_size];
    sprintf(castpath, "%s", path);
    
    char fpath[buffer_size];

    //sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));
    sprintf(fpath, "%s%s/%s", dirpath, get_dir_name(get_decryption_path(castpath)), get_file_name(castpath));

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

    log_info_command("MKNOD", path, NULL);

	return 0;
}
```

```C
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_decryption_path(path));

    printf("Open : %s\n", fpath);

	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);

    log_info_command("OPEN", path, NULL);

	return 0;
}
```

Setelah fungsi dasar telah dijelaskan, selanjutnya adalah membahas mengenai cara kerja soal nomer 1.
1. Kita check pada fungsi `get_encryption_mode` jika menghasilkan `1 << 0` berarti tandanya directory / file ini harus di encode dengan menggunakan Atbash.
2. Namun ketika directory tersebut di-rename menjadi yang tidak memiliki awalan `AtoZ_` maka ketika dipanggil ulang dengan `get_encryption_mode` akan menghasilkan `0`, sehingga tidak perlu menggunakan enkripsi sama sekali.
3. Hal ini berlaku pada saat directory baru dibuat atau saat di-rename karena kami baru melakukan enkripsi hanya saat masuk didalam directory yang bersangkutan tersebut sehingga akan di encode dengan fungsi `readdir` dari FUSE yang telah kami modifikasi.
4. Hal ini juga berlaku pada directory rekursif karena enkripsi terjadi ketika FUSE `readdir` dipanggil.

### Kendala
Meskipun kita telah berusaha untuk membenarkan fungsi `get_attr`, tetap saja terdapat error / pemberitahuan pada layar console. Hal ini diketahui karena ketika melakukan decode pada `get_attr`, decode tidak berjalan dengan sempurna. Meskipun muncul error atau pemberitahuan perintah masih berhasil di eksekusi dan menampilkan atau melakukan sesuai perintah yang diinginkan.

## Soal 2 (RX_folder)

### Tujuan

Membuat sistem FUSE pada path `/home/[USER]/Downloads` yang dapat digunakan untuk encode dan decode. Ketika suatu directory memiliki substring "RX_" pada awal nama directory-nya maka directory tersebut akan di encode menggunakan Atbash Cypher dan dilanjut dengan ROT13 Cypher. Ketika ketika terdapat directory yang di rename menjadi `RX_` maka akan diencode menggunakan Atbash Cypher dan dilanjutkan dengan Vignere Cypher dengan string "SISOP" sebagai key nya. Namun ketika directory tersebut di rename menjadi nama yang tidak diawali dengan "RX_" maka directory tersebut akan di decode. Terdapat ketentuan juga bahwa encode an yang terjadi berjalan secara recursive, jadi file atau folder yang di dalam directory tersebut akan ikut ter-encode.

### Pengerjaan

Pengerjaan nomer 2 tidak jauh dengan nomer 1. Namun terdapat beberapa perbedaan, jadi kami hanya akan membahas perbedaan tersebut.

Sebelum itu berikut merupakan penjelasan dari algoritma enkripsi ROT13 dan Vignere.

--- PENJELASAN ROT13 ---

--- PENJELASAN Vignere ---

Ketika suatu directory memiliki awalan `RX_` maka directory tersebut dapat memiliki 2 kemungkinan encoding yang dapat terjadi.
1. Encoding menggunakan Atbash + ROT13 jika directory baru dibuat dengan awalan `RX_`.
2. Encoding menggunakan Atbash + Vignere dengan key="SISOP" jika directory di-rename dengan awalan `RX_`.

Untuk mengatasi kompleksitas ini, kami memutuskan untuk membuat flag sebuah hidden files (file yang memiliki character `.` pada awal namanya). Untuk mendeteksi suatu directory `RX_` menggunakan encoding pertama maka di dalam directory tersebut ada suatu file yang bernama `.status_rx_1`. Sedangkan untuk mendeteksi suatu directory `RX_` menggunakan encoding kedua maka di dalam directory tersebut ada suatu file yang bernama `.status_rx_2`.

Berikut merupakan fungsi untuk mendeteksi jenis encoding `RX_`.

```C
// 0 == none, 1 == rx 1, 2 == rx 2
int get_rx_mode(char *path){
    char fpath[buffer_size];
    sprintf(fpath, "%s/%s", path, ".status_rx_1");
    // sprintf(fpath, "%s%s/%s", dirpath, path, ".status_rx_1");

    printf("check rx status : %s\n", fpath);

    if (access(fpath, F_OK) == 0){
        //printf("rx value : %d\n", 1);
        return 1;
    }

    // sprintf(fpath, "%s%s/%s", dirpath, path, ".status_rx_2");
    sprintf(fpath, "%s/%s", path, ".status_rx_2");
    if (access(fpath, F_OK) == 0){
        //printf("rx value : %d\n", 2);
        return 2;
    }

    return 0;
}
```

Fungsi diatas akan digunakan pada fungsi berikut.

```C
//mode << 0 == none,1 << 0 = atoz,1 << 1 = rx_1, << 2 = rx_2, << 3 = special
int get_encryption_mode(char *path){
    if (strlen(path) == 0){
        return 0;
    }
    char *temp = path;

    int mode = 0;

    //printf("REC : %s -> %d\n", fname, mode);
    char *tok = strtok(temp, "/");
    char fpath[buffer_size];
    fpath[0] = 0;

    sprintf(fpath, "%s", dirpath);

    while (tok){
        sprintf(fpath + strlen(fpath), "/%s", tok);

        int rx_mode = get_rx_mode(fpath);

        printf("kok ngebug : %d\n", rx_mode);

        if (strstr(tok, "A_is_a_") == tok){
            mode |= 1 << 3;
            return mode;
        }
        else if (strstr(tok, "AtoZ_") == tok){
            mode |= 1 << 0;
        } else if (strstr(tok, "RX_") == tok && rx_mode == 1){
            mode |= 1 << 1;
        } else if (strstr(tok, "RX_") == tok && rx_mode == 2){
            mode |= 1 << 2;
        }
        tok = strtok(NULL, "/");
    }

    return mode;
}
```

Dengan hal ini masalah untuk menentukan jenis enkripsi `RX_` dapat diselesaikan.

### Kendala
1. Meskipun kita telah berusaha untuk membenarkan fungsi `get_attr`, tetap saja terdapat error / pemberitahuan pada layar console. Hal ini diketahui karena ketika melakukan decode pada `get_attr`, decode tidak berjalan dengan sempurna. Meskipun muncul error atau pemberitahuan perintah masih berhasil di eksekusi dan menampilkan atau melakukan sesuai perintah yang diinginkan.
2. Pada nomer 2e belum dapat diselesaikan karena kurangnya waktu dan ide cara pengerjaan yang dapat digunakan untuk mengatasi hal tersebut. Meskipun begitu terdapat ide untuk melakukan manipulasi file pada bagian `readdir` tetapi karena kurang petunjuk kami ragu untuk mengimplementasikannya.

## Soal 3 (A_is_a_folder)

### Tujuan

Membuat system FUSE yang dapat melakukan decode terhadap seluruh file yang berada dalam directory yang memiliki nama yang diawali dengan substring `A_is_a_`. Seluruh file atau folder yang telah di decode tersebut akan di encode lagi dengan ketentuan tertentu yang akan dijelaskan nanti. 

### Pengerjaan

### Kendala

## Soal 4

### Tujuan (Catatan Log)

Membuat log system yang akan mendata setiap system call yang digunakan. Tujuannya untuk memonitor kegiatan pada filesystem.

### Pengerjaan

Untuk menyelesaikan tugas berikut ini, kita harus membuat 2 log level pada log system, yakni WARNING dan INFO. Bentuk kode yang akan dibuat untuk kedua log level ini bisa dikatakan mirip, sehingga kita akan mengggunakan dasar pembuatan yang sama.
Pertama, buat 2 fungsi void, yakni log_info_command dan log_warning_command, isi dari kedua fungsi ini sama, yakni (char* command, const char* from, const char* to). Berikutnya, masukkan variabel time_t untuk menunjukkan waktu pada log, struct tm tm untuk menunjukkan waktu sekarang dan string main [1000].
```C
int log_info_command(char *command, const char *from, const char *to){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
  ```
```C
int log_warning_command(char *command, const char *from, const char *to){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
```
Langkah kedua adalah membuat perulangan if untuk menjalankan logging. kita akan membuat syarat perulangan ini adalah keberadaan tempat tujuan (const char* to). Jika tidak ada tujuan logging, maka buat format logging :
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC]
dengan DESC = const char* from (asal folder).
Jika tujuan logging diketahui/ tidak bernilai NULL, maka buat format logging :
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]
dengan kedua DESC tersebut berisi const char* from dan const char* to.
Jangan lupa untuk membedakan level pada format sesuai dengan log level masing-masing.
```C
if(to == NULL){
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
```
```C
  if(to == NULL){
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
```
Terakhir, print hasil dari perulangan ini (dalam bentuk string main). Lalu buka file foutput dan masukkan string main ke dalamnya. Lalu return.
```C
	printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}
```

### Kendala
Tidak ada kendala untuk soal ini.

# Referensi
1. asd
2. asd
3. asd
4. asd
