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
