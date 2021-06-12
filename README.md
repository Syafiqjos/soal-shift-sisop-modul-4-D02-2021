# soal-shift-sisop-modul-4-D02-2021
- Jessica Tasyanita (05111940000043)
- Ahmad Syafiq Aqil Wafi (05111940000089)
- Maximilian H M Lingga (05111940000092)

Finally modul terakhir..
Commit pertama setelah revisi

## Soal 4
### Tujuan
Membuat log system yang akan mendata setiap system call yang digunakan. Tujuannya untuk memonitor kegiatan pada filesystem.

### Pengerjaan
Untuk menyelesaikan tugas berikut ini, kita harus membuat 2 log level pada log system, yakni WARNING dan INFO. Bentuk kode yang akan dibuat untuk kedua log level ini bisa dikatakan mirip, sehingga kita akan mengggunakan dasar pembuatan yang sama.
Pertama, buat 2 fungsi void, yakni log_info_command dan log_warning_command, isi dari kedua fungsi ini sama, yakni (char* command, const char* from, const char* to). Berikutnya, masukkan variabel time_t untuk menunjukkan waktu pada log, struct tm tm untuk menunjukkan waktu sekarang dan string main [1000].
```
int log_info_command(char *command, const char *from, const char *to){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
  ```
  ```
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
```
if(to == NULL){
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
```
```
  if(to == NULL){
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }else{
        sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
	tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
```
Terakhir, print hasil dari perulangan ini (dalam bentuk string main). Lalu buka file foutput dan masukkan string main ke dalamnya. Lalu return.
```printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}
```
