# PRAKTIKUM MODUL 03 SISTEM OPERASI
## Kelompok IT20
### Anggota Kelompok :
|             Nama              |     NRP    |
|-------------------------------|------------|
| Revalina Fairuzy Azhari Putri | 5027231001 |
| Farida Qurrotu 'Ayuna         | 5027231015 |
| Gallant Damas Hayuaji         | 5027231037 |

## Soal 1

Pada zaman dahulu pada galaksi yang jauh-jauh sekali, hiduplah seorang Stelle. Stelle adalah seseorang yang sangat tertarik dengan Tempat Sampah dan Parkiran Luar Angkasa. Stelle memulai untuk mencari Tempat Sampah dan Parkiran yang terbaik di angkasa. Dia memerlukan program untuk bisa secara otomatis mengetahui Tempat Sampah dan Parkiran dengan rating terbaik di angkasa. Programnya berbentuk microservice sebagai berikut:

a) Dalam auth.c pastikan file yang masuk ke folder new-entry adalah file csv dan berakhiran  trashcan dan parkinglot. Jika bukan, program akan secara langsung akan delete file tersebut. Contoh dari nama file yang akan diautentikasi:

   - belobog_trashcan.csv
   - osaka_parkinglot.csv

b) Format data (Kolom)  yang berada dalam file csv adalah seperti berikut:

![Screenshot from 2024-05-11 17-00-30](https://github.com/Faridaqurr/lapres/assets/150933246/39dd4511-abb0-4bd0-bbe4-ecce56dc44cd)

atau

![Screenshot from 2024-05-11 17-00-50](https://github.com/Faridaqurr/lapres/assets/150933246/3ff99191-f9eb-4c27-ad6a-71be18a31e65)

c) File csv yang lolos tahap autentikasi akan dikirim ke shared memory. 

d) Dalam rate.c, proses akan mengambil data csv dari shared memory dan akan memberikan output Tempat Sampah dan Parkiran dengan Rating Terbaik dari data tersebut.

![Screenshot from 2024-05-11 17-02-35](https://github.com/Faridaqurr/lapres/assets/150933246/201843ae-69c0-4daf-80ef-dc9ac92c97bc)

e) Pada db.c, proses bisa memindahkan file dari new-data ke folder microservices/database, WAJIB MENGGUNAKAN SHARED MEMORY.

f) Log semua file yang masuk ke folder microservices/database ke dalam file db.log dengan contoh format sebagai berikut:
[DD/MM/YY hh:mm:ss] [type] [filename]

ex : `[07/04/2024 08:34:50] [Trash Can] [belobog_trashcan.csv]

Contoh direktori awal:

![Screenshot from 2024-05-11 17-09-58](https://github.com/Faridaqurr/lapres/assets/150933246/a9e9954d-63c4-402c-8ebb-7568502047bd)

Contoh direktori akhir setelah dijalankan auth.c dan db.c:

![Screenshot from 2024-05-11 17-10-26](https://github.com/Faridaqurr/lapres/assets/150933246/b003a07b-c4ff-4f51-af9c-05efd2baac8a)

## Jawab

### soal 1a

Pada soal ini diminta untuk membuat sebuah program yang mengautentikasi file csv dengan syarat file berakhiran _parkinglot_ atau _trashacan_ dan menghapus file yang tidak memenuhi syarat autentikasi
      
      #define SHARED_MEM_KEY 1234
      #define MAX_FILENAME_LENGTH 256

`#define SHARED_MEM_KEY 1234` untuk mendefinisikan kunci/token dari shared memory yang dipakai program ini

`#define MAX_FILENAME_LENGTH 256` untuk mendefinisikan panjang karakter nama file adalah 256 dan menghindari terjadinya perulangan angka 256 di tempat yang berbeda
      
      struct FileData {
          char filename[MAX_FILENAME_LENGTH];
      };

`char filename[MAX_FILENAME_LENGTH];` untuk menyimpan informasi mengenai semua file agar mudah untuk dikelola

      int isFileAuthenticated(char *filename) {
          int len = strlen(filename);
          if (len >= 12 && strcmp(filename + len - 12, "trashcan.csv") == 0) {
              return 1; 
          } else if (len >= 14 && strcmp(filename + len - 14, "parkinglot.csv") == 0) {
              return 1; 
          }
          return 0; 
      }

`if (len >= 12 && strcmp(filename + len - 12, "trashcan.csv") == 0)` untuk syarat autentikasi file jika berakhiran trashcan.csv dimana berjumlah 12 karakter

`else if (len >= 14 && strcmp(filename + len - 14, "parkinglot.csv") == 0)` untuk syarat autentikasi file jika berakhiran parkinglot.csv dimana berjumlah 14 karakter

`return 1` untuk menunjukkan indikasi bahwa file memenuhi syarat autentukasi

`return 0` untuk menunjukkan indikasi bahwa file tidak memenuhi syarat autentikasi

      void deleteFile(char *filepath) {
          pid_t pid = fork();
          if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
          } else if (pid == 0) { 
              if (remove(filepath) == 0) {
                  printf("File %s tidak memenuhi syarat autentikasi dan telah dihapus.\n", filepath);
              } else {
                  perror("remove");
              }
              exit(EXIT_SUCCESS);
          } else { 
              wait(NULL); // 
          }
      }

Fungsi diatas digunakan untuk menghapus file yang tidak memenuhi syarat autentikasi

`pid_t pid = fork();` untuk memanggil fungsi sistem fork

`if (pid < 0)` untuk kondisi jika gagal membuat proses baru

`else if (pid == 0)` untuk kondisi pembuatan proses baru berhasil dibuat

`wait(NULL);` untuk eksekusi proses induk agar menunggu proses childnya selesai berproses dahulu

          if (shmdt(shm_ptr) == -1) {
              perror("shmdt");
              exit(EXIT_FAILURE);
          }
          
 Fungsi diatas untuk melepaskan shared memory setelah proses program selesai agar tidak terjadi kebocoran data 

### soal 1b

Pada soal ini format dalam file csv nya harus sesuai dengan ketentuan soal

![Screenshot from 2024-05-11 19-13-02](https://github.com/Faridaqurr/lapres/assets/150933246/44062f90-113a-4344-8f8f-2ef39783d7be)

### soal 1c

Pada soal ini program auth.c mengirimkan nama file csv yang lolos autentikasi ke shared memory
      
      void copyFilenameToSharedMemory(char *shm_ptr, char *filename) {
          strcpy(shm_ptr, filename);
      }
      
Fungsi diatas menyalin nama file yang lolos autentikasi ke dalam shared memory

`strcpy(shm_ptr, filename);` untuk menyalin string dari _filename_ ke _shm_ptr_ 
      
      int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, IPC_CREAT | 0666);
          if (shmid == -1) {
              perror("shmget");
              exit(EXIT_FAILURE);
          }
          char *shm_ptr = shmat(shmid, NULL, 0);
          if (shm_ptr == (char *)-1) {
              perror("shmat");
              exit(EXIT_FAILURE);
          }

Fungsi diatas membuat shared memory 

`0666` untuk token akses shared memory

### soal 1d

pada soal ini membuat program rate.c yang akan mengambil data csv dari shared memory dan akan memberi output tempat sampah dan tempat parkir rating terbaik dari data yang diambil

      struct Place {
          char name[MAX_FILENAME_LENGTH];
          float rating;
      };

Fungsi diatas untuk menyimpan informasi tentang tempat sampah dan tempat parkir seperti nama dan juga rating

`char name[MAX_FILENAME_LENGTH];` untuk  mendeklarasikan sebuah array karakter dengan nama _name_ yang dapat menampung string dengan panjang maksimum yang telah di define

`float rating;` untuk mendeklarasikan variabel _rating_ bertipe float yang menyimpan data rating dari file csv

      struct Place findBestPlace(FILE *file) {
          struct Place bestPlace = {"", 0.0};
          char line[MAX_FILENAME_LENGTH];
          fgets(line, sizeof(line), file);
      
          while (fgets(line, sizeof(line), file) != NULL) {
              char name[MAX_FILENAME_LENGTH];
              float rating;
              sscanf(line, "%[^,],%f", name, &rating);
      
              if (rating > bestPlace.rating) {
                  strcpy(bestPlace.name, name);
                  bestPlace.rating = rating;
              }
          }
          return bestPlace;
      }

Fungsi diatas untuk mencari tempat dengan rating tertinggi

`struct Place bestPlace = {"", 0.0};` untuk menyimpan informasi tempat dengan rating tertinggi dengan mengatur nama tempat kosong dan rating 0.0 

`fgets(line, sizeof(line), file);` untuk membaca dan membuang baris pertama atau baris header dalam file, seperti baris yang berisikan nama, rating

`sscanf(line, "%[^,],%f", name, &rating);` untuk membaca isi file dengan penjelasan:
- *%[^,]*  dan  *name* untuk membaca string hingga bertemu tanda koma dan akan menyimpannya ke dalam variabel name

- *,%f*  dan  *&rating* untuk  membaca nilai float setelah tanda koma dan akan menyimpannya ke dalam variabel rating

`while (fgets(line, sizeof(line), file) != NULL)` untuk membaca data dari file csv

      int main(int argc, char *argv[]) {
          if (argc != 2) {
              fprintf(stderr, "Usage: %s <filename.csv>\n", argv[0]);
              exit(EXIT_FAILURE);
          }
          
`if (argc != 2)` untuk menjalankan program rate.c ini harus menggunakan argumen seperti ./rate <nama filenya> 

       int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);
          if (shmid == -1) {
              perror("shmget");
              exit(EXIT_FAILURE);
          }
          char *shm_ptr = shmat(shmid, NULL, 0);
          if (shm_ptr == (char *)-1) {
              perror("shmat");
              exit(EXIT_FAILURE);
          }

`int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);` untuk akses shared memory

`0666` untuk token shared memory yang digunakan oleh auth.c dan db.c
      
          char filename[MAX_FILENAME_LENGTH];
          strcpy(filename, argv[1]);
      
          printf("Type: ");
          if (strstr(filename, "trashcan") != NULL) {
              printf("Trash Can\n");
          } else if (strstr(filename, "parkinglot") != NULL) {
              printf("Parking Lot\n");
          } else {
              printf("Unknown\n");
          }
          printf("Filename: %s\n", filename);
          printf("---------------------------------------\n");
      
          char filepath[MAX_FILENAME_LENGTH + 20]; 
          sprintf(filepath, "./database/%s", filename);
          FILE *file = fopen(filepath, "r");
          if (file == NULL) {
              perror("fopen");
              exit(EXIT_FAILURE);
          }
      
          struct Place bestPlace = findBestPlace(file);
          printf("Name: %s\n", bestPlace.name);
          printf("Rating: %.1f\n", bestPlace.rating);
      
          fclose(file);
          if (shmdt(shm_ptr) == -1) {
              perror("shmdt");
              exit(EXIT_FAILURE);
          }
      
          return 0;
      }

`strcpy(filename, argv[1]);` untuk membaca nama file yang ingin di proses

`printf("Type: ");` untuk penulisan output Type filenya

`printf("Filename: %s\n", filename);` untuk penulisan output Filename

`sprintf(filepath, "./database/%s", filename);` akses file pada folder database

`printf("Name: %s\n", bestPlace.name);` untuk penulisan Name dari nama tempat dengan rating terbaik

`printf("Rating: %.1f\n", bestPlace.rating);` untuk penulisan Rating dari rating tertinggi

`if (shmdt(shm_ptr) == -1)` untuk melepaskan shared memory ketika program sudah selesai

### soal 1e

Pada soal ini diminta untuk membuat prgram db.c yang dapat memindahkan file dari new-data ke folder microservices/database menggunakan shared memory
      
      #define SHARED_MEM_KEY 1234
      #define MAX_FILENAME_LENGTH 256
      #define LOG_FILE "database/db.log"

`#define SHARED_MEM_KEY 1234` untuk akses shared memory

`#define LOG_FILE "database/db.log"` untuk mendefinisikan pembuatan db.log di dalam direktori database
      
      struct FileData {
          char filename[MAX_FILENAME_LENGTH];
      };
      
`char filename[MAX_FILENAME_LENGTH];` untuk menyimpan informasi mengenai semua file agar mudah untuk dikelola

      void copyFilenameFromSharedMemory(char *shm_ptr, char *filename) {
          strcpy(filename, shm_ptr);
      }
      
`strcpy(shm_ptr, filename);` untuk menyalin string dari _filename_ ke _shm_ptr_ 

      void moveFile(char *filename) {
          char oldpath[MAX_FILENAME_LENGTH + 20]; 
          sprintf(oldpath, "../new-data/%s", filename);
          char newpath[MAX_FILENAME_LENGTH + 30];
          sprintf(newpath, "./database/%s", filename);
      
          mkdir("./database", 0777);
      
          if (rename(oldpath, newpath) == 0) {
              printf("File %s berhasil dipindahkan ke microservices/database.\n", filename);
          } else {
              perror("rename");
          }
      }
      
Fungsi diatas untuk memindahkan file ke folder database

`sprintf(oldpath, "../new-data/%s", filename);` untuk akses ke folder new-data

`sprintf(newpath, "./database/%s", filename);` untuk akses ke folder database

`mkdir("./database", 0777);` untuk membuat folder database jika belum ada di dalam microservices

      char* determineFileType(char *filename) {
          if (strstr(filename, "trashcan") != NULL) {
              return "Trash Can";
          } else if (strstr(filename, "parkinglot") != NULL) {
              return "Parking Lot";
          } else {
              return "Unknown";
          }
      }

Fungsi diatas untuk menentukan Type file berdasarkan akhiran nama file

`if (strstr(filename, "trashcan") != NULL)` untuk Type file yang berakhiran trashcan

`else if (strstr(filename, "parkinglot") != NULL)` untuk Type file yang berakhiran parking lot

`return "Unknown";` untuk kondisi jika tidak ada file yang berakhiran trashcan atau parkinglot
       
          int main() {
          int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);
          if (shmid == -1) {
              perror("shmget");
              exit(EXIT_FAILURE);
          }
          char *shm_ptr = shmat(shmid, NULL, 0);
          if (shm_ptr == (char *)-1) {
              perror("shmat");
              exit(EXIT_FAILURE);
          }
Fungsi untuk mengakses shared memory dari program auth.c

`0666` token yang digunakan untuk akses shared memory

          if (shmdt(shm_ptr) == -1) {
              perror("shmdt");
              exit(EXIT_FAILURE);
          }
Fungsi untuk melepas shared memory jika proses sudah selesai dijalankan

### soal 1f

Pada soal ini diminta untuk membuat log file yang masuk ke folder microservices/database ke dalam file db.log dengan contoh format [DD/MM/YY hh:mm:ss] [type] [filename]
      
      void writeLog(char *type, char *filename) {
          FILE *log_file = fopen(LOG_FILE, "a");
          if (log_file != NULL) {
              time_t now = time(NULL);
              struct tm *tm_info = localtime(&now);
              char timestamp[20];
              strftime(timestamp, 20, "%d/%m/%Y %H:%M:%S", tm_info);
              fprintf(log_file, "[%s] [%s] [%s]\n", timestamp, type, filename);
              fclose(log_file);
          } else {
              perror("fopen");
          }
      }

Fungsi untuk menuliskan log ke dalam db.log ketika file dari folder new-data dipindahkan ke folder database

`FILE *log_file = fopen(LOG_FILE, "a");` untuk membuka db.log dan melakukan penulisan log di dalamnya, namun jika belum ada db.log di dalam database maka akan otomatis dibuat

`fprintf(log_file, "[%s] [%s] [%s]\n", timestamp, type, filename);` untuk penulisan format dari db.log

`time_t now = time(NULL);` untuk mendapatkan waktu ketika file dipindahkan

`struct tm *tm_info = localtime(&now);` untuk mengonversi waktu dalam bentuk time_t menjadi waktu lokal (dd/mm/yyyy HH:MM:SS)

`char timestamp[20];` untuk menampung hasil dari konversi waktu

## Revisi

Pada code rate.c sebelumnya hanya bisa dijalankan untuk mengeluarkan output per file menggunakan ./rate <nama file>. Sehingga pada revisi ini code diubah agar juga dapat dijalankan untuk mengeluarkan output semua file dengan satu command yaitu ./rate

code rate.c yang diperbarui:
      
      #include <stdio.h>
      #include <stdlib.h>
      #include <string.h>
      #include <sys/ipc.h>
      #include <sys/shm.h>
      #include <dirent.h> 
      
      #define SHARED_MEM_KEY 1234
      #define MAX_FILENAME_LENGTH 256
      
      struct Place {
          char name[MAX_FILENAME_LENGTH];
          float rating;
      };
      
      struct Place findBestPlace(FILE *file) {
          struct Place bestPlace = {"", 0.0};
          // Melewati baris header
          char line[MAX_FILENAME_LENGTH];
          fgets(line, sizeof(line), file);
      
          while (fgets(line, sizeof(line), file) != NULL) {
              char name[MAX_FILENAME_LENGTH];
              float rating;
              sscanf(line, "%[^,],%f", name, &rating);
      
              if (rating > bestPlace.rating) {
                  strcpy(bestPlace.name, name);
                  bestPlace.rating = rating;
              }
          }
          return bestPlace;
      }
      
      void processCSV(char *filename) {
          // Mengakses shared memory yang telah dibuat oleh program auth.c dan db.c
          int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);
          if (shmid == -1) {
              perror("shmget");
              exit(EXIT_FAILURE);
          }
          char *shm_ptr = shmat(shmid, NULL, 0);
          if (shm_ptr == (char *)-1) {
              perror("shmat");
              exit(EXIT_FAILURE);
          }
      
          // Menampilkan output
          printf("\nType: ");
          if (strstr(filename, "trashcan") != NULL) {
              printf("Trash Can\n");
          } else if (strstr(filename, "parkinglot") != NULL) {
              printf("Parking Lot\n");
          } else {
              printf("Unknown\n");
          }
          printf("Filename: %s\n", filename);
          printf("---------------------------------------\n");
      
          // Membuka file CSV yang terletak di folder database
          char filepath[MAX_FILENAME_LENGTH + 20]; // Menyertakan path folder "database"
          sprintf(filepath, "./database/%s", filename);
          FILE *file = fopen(filepath, "r");
          if (file == NULL) {
              perror("fopen");
              exit(EXIT_FAILURE);
          }
      
          // Mencari tempat dengan rating tertinggi dari file CSV dan menampilkannya
          struct Place bestPlace = findBestPlace(file);
          printf("Name: %s\n", bestPlace.name);
          printf("Rating: %.1f\n", bestPlace.rating);
      
          // Menutup file dan melepaskan shared memory
          fclose(file);
          if (shmdt(shm_ptr) == -1) {
              perror("shmdt");
              exit(EXIT_FAILURE);
          }
      }
      
      int main(int argc, char *argv[]) {
          if (argc == 1) {
              // Jika tidak ada argumen yang diberikan, tampilkan output untuk semua file CSV
              DIR *dir;
              struct dirent *ent;
              if ((dir = opendir("./database")) != NULL) {
                  while ((ent = readdir(dir)) != NULL) {
                      if (strstr(ent->d_name, ".csv") != NULL) {
                          processCSV(ent->d_name);
                      }
                  }
                  closedir(dir);
              } else {
                  perror("opendir");
                  return EXIT_FAILURE;
              }
          } else if (argc == 2) {
              // Jika diberikan satu argumen, tampilkan output untuk file CSV yang disebutkan
              processCSV(argv[1]);
          } else {
              // Jika diberikan lebih dari satu argumen, tampilkan pesan kesalahan
              fprintf(stderr, "Usage: %s [filename.csv]\n", argv[0]);
              return EXIT_FAILURE;
          }
      
          return 0;
      }

1)Penambahan fungsi _processCSV_, karena pada code sebelumnya fungsi untuk outputnya dijabarkan di main-nya berbeda pada revisi code ini, pada code ini hanya melakukan pemanggilan fungsi output pada main-nya sedangkan untuk fungsi outputnya sendiri sudah di buat pada fungsi processCSV

     void processCSV(char *filename) {
             int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);
             if (shmid == -1) {
                 perror("shmget");
                 exit(EXIT_FAILURE);
             }
             char *shm_ptr = shmat(shmid, NULL, 0);
             if (shm_ptr == (char *)-1) {
                 perror("shmat");
                 exit(EXIT_FAILURE);
             }
         
             printf("\nType: ");
             if (strstr(filename, "trashcan") != NULL) {
                 printf("Trash Can\n");
             } else if (strstr(filename, "parkinglot") != NULL) {
                 printf("Parking Lot\n");
             } else {
                 printf("Unknown\n");
             }
             printf("Filename: %s\n", filename);
             printf("---------------------------------------\n");
         
             char filepath[MAX_FILENAME_LENGTH + 20]; 
             sprintf(filepath, "./database/%s", filename);
             FILE *file = fopen(filepath, "r");
             if (file == NULL) {
                 perror("fopen");
                 exit(EXIT_FAILURE);
             }
         
             struct Place bestPlace = findBestPlace(file);
             printf("Name: %s\n", bestPlace.name);
             printf("Rating: %.1f\n", bestPlace.rating);
         
             fclose(file);
             if (shmdt(shm_ptr) == -1) {
                 perror("shmdt");
                 exit(EXIT_FAILURE);
             }
         }

`int shmid = shmget(SHARED_MEM_KEY, MAX_FILENAME_LENGTH, 0666);` untuk akses shared memory

`printf("\nType: ");` untuk penulisan output Type file

`printf("Filename: %s\n", filename);` untuk penulisan output Filename

`sprintf(filepath, "./database/%s", filename);` untuk akses file di database

`printf("Name: %s\n", bestPlace.name);` untuk penulisan Name dari nama tempat dengan rating terbaik

`printf("Rating: %.1f\n", bestPlace.rating);` untuk penulisan Rating dari rating tertinggi

`if (shmdt(shm_ptr) == -1)` untuk melepaskan shared memory ketika program sudah selesai

2)Pengubahan di dalam main, karena isi main code sebelumnya adalah penjabaran untuk tampilan outputnya maka pada main code revisi ini hanya ada pemanggilan fungsi processCSV dan juga cara menjalankan code revisi ini sekarang bisa menggunakan argumen (nama file) atau tanpa argumen

      int main(int argc, char *argv[]) {
          if (argc == 1) {
              DIR *dir;
              struct dirent *ent;
              if ((dir = opendir("./database")) != NULL) {
                  while ((ent = readdir(dir)) != NULL) {
                      if (strstr(ent->d_name, ".csv") != NULL) {
                          processCSV(ent->d_name);
                      }
                  }
                  closedir(dir);
              } else {
                  perror("opendir");
                  return EXIT_FAILURE;
              }
          } else if (argc == 2) {
              processCSV(argv[1]);
          } else {
              // Jika diberikan lebih dari satu argumen, tampilkan pesan kesalahan
              fprintf(stderr, "Usage: %s [filename.csv]\n", argv[0]);
              return EXIT_FAILURE;
          }
      
          return 0;
      }

Jika pada code sebelumnya hanya ada `if (argc != 2)` untuk memunculkan output sesuai argumen yang diberikan (nama filenya), maka pada code revisinya menjadi:
   
   - `if (argc == 1)` jika tidak ada argumen yang ditambahkan maka akan menampilkan semua output file
     
   - `else if (argc == 2)` jika diberikan argumen (nama file) maka hanya akan menampilkan output dari file yang diminta
     
   - `else` jika argumen yang dimasukkan lebih, maka akan memunculkan pesan kesalahan

## Hasil Output

![Screenshot from 2024-05-10 04-32-36](https://github.com/Faridaqurr/lapres/assets/150933246/4326ede1-216d-4c38-b996-3c5f60ca54f1)

![Screenshot from 2024-05-10 04-27-58](https://github.com/Faridaqurr/lapres/assets/150933246/5492e967-977a-4561-a30b-c7b0f4b18dbc)

![Screenshot from 2024-05-10 04-29-19](https://github.com/Faridaqurr/lapres/assets/150933246/6c9d3264-2629-48c4-8624-a42aa5175f4e)


## Soal 2
Max Verstappen 🏎️ seorang pembalap F1 dan programer memiliki seorang adik bernama Min Verstappen (masih SD) sedang menghadapi tahap paling kelam dalam kehidupan yaitu perkalian matematika, Min meminta bantuan Max untuk membuat kalkulator perkalian sederhana (satu sampai sembilan). Sembari Max nguli dia menyuruh Min untuk belajar perkalian dari web (referensi) agar tidak bergantung pada kalkulator.
(Wajib menerapkan konsep pipes dan fork seperti yang dijelaskan di modul Sisop. Gunakan 2 pipes dengan diagram seperti di modul 3).

a. Sesuai request dari adiknya Max ingin nama programnya dudududu.c. Sebelum program parent process dan child process, ada input dari user berupa 2 string. Contoh input: tiga tujuh. 

b. Pada parent process, program akan mengubah input menjadi angka dan melakukan perkalian dari angka yang telah diubah. Contoh: tiga tujuh menjadi 21. 

c. Pada child process, program akan mengubah hasil angka yang telah diperoleh dari parent process menjadi kalimat. Contoh: `21` menjadi “dua puluh satu”.

d. Max ingin membuat program kalkulator dapat melakukan penjumlahan, pengurangan, dan pembagian, maka pada program buatlah argumen untuk menjalankan program : 

  i. perkalian	: ./kalkulator -kali
  
  ii. penjumlahan	: ./kalkulator -tambah
  
  iii. pengurangan	: ./kalkulator -kurang
  
  iv. pembagian	: ./kalkulator -bagi
  
Beberapa hari kemudian karena Max terpaksa keluar dari Australian Grand Prix 2024 membuat Max tidak bersemangat untuk melanjutkan programnya sehingga kalkulator yang dibuatnya cuma menampilkan hasil positif jika bernilai negatif maka program akan print “ERROR” serta cuma menampilkan bilangan bulat jika ada bilangan desimal maka dibulatkan ke bawah.

e. Setelah diberi semangat, Max pun melanjutkan programnya dia ingin (pada child process) kalimat akan di print dengan contoh format : 

  i. perkalian	: “hasil perkalian tiga dan tujuh adalah dua puluh satu.”
  ii. penjumlahan	: “hasil penjumlahan tiga dan tujuh adalah sepuluh.”
  iii. pengurangan	: “hasil pengurangan tujuh dan tiga adalah empat.”
  iv. pembagian	: “hasil pembagian tujuh dan tiga adalah dua.”
  
f. Max ingin hasil dari setiap perhitungan dicatat dalam sebuah log yang diberi nama histori.log. Pada parent process, lakukan pembuatan file log berdasarkan data yang dikirim dari child process. 

Format: [date] [type] [message]

Type: KALI, TAMBAH, KURANG, BAGI

Ex:

[10/03/24 00:29:47] [KALI] tujuh kali enam sama dengan empat puluh dua.

[10/03/24 00:30:00] [TAMBAH] sembilan tambah sepuluh sama dengan sembilan belas.

[10/03/24 00:30:12] [KURANG] ERROR pada pengurangan.

## Jawab

Pada soal ini, saya membuat konfigurasi program sesuai yang diminta pada soal dengan nama `dudududu.c`
Berikut adalah code nya :
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Function to convert number to words
void convert_to_words(char *num, char *output) {
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};

    int len = strlen(num);
    if (len == 0) {
        strcpy(output, "");
        return;
    }
    if (len > 2) {
        strcpy(output, "ERROR");
        return;
    }

    // Single digit
    if (len == 1) {
        strcpy(output, ones[*num - '0']);
        return;
    }

    // Two digits
    if (num[0] == '1') {
        switch (num[1]) {
            case '0': strcpy(output, "sepuluh"); break;
            case '1': strcpy(output, "sebelas"); break;
            case '2': strcpy(output, "dua belas"); break;
            case '3': strcpy(output, "tiga belas"); break;
            case '4': strcpy(output, "empat belas"); break;
            case '5': strcpy(output, "lima belas"); break;
            case '6': strcpy(output, "enam belas"); break;
            case '7': strcpy(output, "tujuh belas"); break;
            case '8': strcpy(output, "delapan belas"); break;
            case '9': strcpy(output, "sembilan belas"); break;
        }
    } else {
        char *tens[] = {"", "sepuluh", "dua puluh", "tiga puluh", "empat puluh", "lima puluh", "enam puluh", "tujuh puluh", "delapan puluh", "sembilan puluh"};
        strcpy(output, tens[num[0] - '0']);
        strcat(output, " ");
        strcat(output, ones[num[1] - '0']);
    }
}

// Function to convert words to number
int convert_to_number(char *word) {
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
    int num = -1;

    for (int i = 0; i < 10; i++) {
        if (strcmp(word, ones[i]) == 0) {
            num = i;
            break;
        }
    }

    return num;
}

// Function to calculate result and convert to words
void calculate_result(int num1, int num2, char *operator, char *output) {
    int result;

    if (strcmp(operator, "-kali") == 0) {
        result = num1 * num2;
    } else if (strcmp(operator, "-tambah") == 0) {
        result = num1 + num2;
    } else if (strcmp(operator, "-kurang") == 0) {
        result = num1 - num2;
    } else if (strcmp(operator, "-bagi") == 0) {
        if (num2 == 0) {
            strcpy(output, "ERROR");
            return;
        }
        result = num1 / num2;
    } else {
        strcpy(output, "ERROR");
        return;
    }

    if (result < 0) {
        strcpy(output, "ERROR");
        return;
    }

    sprintf(output, "%d", result);
}

// Function to log history
void log_history(char *operation, char *num1_str, char *num2_str, char *result_str, char *argv) {
    FILE *log_file = fopen("histori.log", "a");
    if (log_file == NULL) {
        perror("Error opening file");
        return;
    }

    time_t rawtime;
    struct tm *info;
    char timestamp[20];

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", info);

    // Convert operation to uppercase
    char upper_operation[20];
    if (strcmp(operation, "-kali") == 0) {
        strcpy(upper_operation, "KALI");
    } else if (strcmp(operation, "-tambah") == 0) {
        strcpy(upper_operation, "TAMBAH");
    } else if (strcmp(operation, "-kurang") == 0) {
        strcpy(upper_operation, "KURANG");
    } else if (strcmp(operation, "-bagi") == 0) {
        strcpy(upper_operation, "BAGI");
    }

    if (strcmp(result_str, "ERROR") == 0) {
    fprintf(log_file, "[%s] [%s] ERROR pada %s.\n", timestamp, upper_operation, argv + 1);
} else {
    fprintf(log_file, "[%s] [%s] %s %s %s sama dengan %s.\n", timestamp, upper_operation, num1_str, argv + 1, num2_str, result_str);
}

    fclose(log_file);
}


int main(int argc, char *argv[]) {
    // Check if argument is provided
    if (argc != 2) {
        printf("Usage: %s <operation>\n", argv[0]);
        return 1;
    }

    // Check if operation is valid
    if (strcmp(argv[1], "-kali") != 0 && strcmp(argv[1], "-tambah") != 0 && strcmp(argv[1], "-kurang") != 0 && strcmp(argv[1], "-bagi") != 0) {
        printf("Invalid operation\n");
        return 1;
    }

    // Input from user
    char input[100];
    printf("Masukkan dua angka (satu sampai sembilan): ");
    fgets(input, sizeof(input), stdin);
    strtok(input, "\n"); // Remove trailing newline

    // Parsing input into two numbers
    char num1_str[10], num2_str[10];
    sscanf(input, "%s %s", num1_str, num2_str);

    // Convert words to numbers
    int num1 = convert_to_number(num1_str);
    int num2 = convert_to_number(num2_str);

    // Check if conversion successful
    if (num1 == -1 || num2 == -1) {
        printf("Invalid input\n");
        return 1;
    }

    // Create pipes
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        // Close read end of pipe
        close(pipefd[0]);

        // Convert result to words
        char result_str[100];
        calculate_result(num1, num2, argv[1], result_str);

        // Convert result to words
        char result_words[100];
        convert_to_words(result_str, result_words);

        // Send result to parent process
        write(pipefd[1], result_words, strlen(result_words) + 1);

        // Close write end of pipe
        close(pipefd[1]);

        exit(0);
    } else { // Parent process
        // Close write end of pipe
        close(pipefd[1]);

        // Read result from child process
        char result[100];
        read(pipefd[0], result, sizeof(result));

        // Close read end of pipe
        close(pipefd[0]);

        // Log history
log_history(argv[1], num1_str, num2_str, result, argv[1]);

        // Print result with quotes
if (strcmp(argv[1], "-kali") == 0) {
    printf("\"Hasil perkalian %s dan %s adalah %s.\"\n", num1_str, num2_str, result);
} else if (strcmp(argv[1], "-tambah") == 0) {
    printf("\"Hasil penjumlahan %s dan %s adalah %s.\"\n", num1_str, num2_str, result);
} else if (strcmp(argv[1], "-kurang") == 0) {
    printf("\"Hasil pengurangan %s dan %s adalah %s.\"\n", num1_str, num2_str, result);
} else if (strcmp(argv[1], "-bagi") == 0) {
    printf("\"Hasil pembagian %s dan %s adalah %s.\"\n", num1_str, num2_str, result);
}
        return 0;
    }
}
```

`convert_to_words` : untuk mengonversi angka yang diberikan dalam bentuk string menjadi kata-kata Bahasa Indonesia. Misalnya, jika diberikan "11", maka akan menghasilkan "sebelas". Hasil konversi disimpan dalam string output.

`convert_to_number` : melakukan kebalikan dari fungsi sebelumnya. Mengonversi kata-kata Bahasa Indonesia menjadi angka. Misalnya, jika diberikan "satu", maka akan menghasilkan angka 1.

`calculate_result` : menerima dua angka dan operator aritmatika, kemudian menghitung hasil operasi aritmatika tersebut. Hasilnya disimpan dalam string output. Jika terjadi kesalahan dalam operasi, seperti pembagian dengan nol, maka hasilnya akan "ERROR".

`log_history` : mencatat operasi yang dilakukan ke dalam file "histori.log". Ini mencatat waktu operasi, jenis operasi, angka yang dioperasikan, dan hasilnya. Jika hasil operasi adalah "ERROR", maka akan dicatat bahwa terjadi kesalahan.

`main` : merupakan fungsi utama program, dimana mengambil argumen dari baris perintah, meminta input dua angka dari pengguna, menjalankan operasi aritmatika, mengonversi hasilnya ke dalam kata-kata, mencetak hasilnya dengan format yang ditentukan, dan akhirnya mencatat operasi tersebut dalam log.

Selanjutnya saya menggunakan command `gcc dudududu.c -o kalkulator` untuk mengexcecuteable program `dudududu.c` agar bisa dijalankan dan terbaca sebagai program `kalkulator`

lalu saya mulai menjalankan programnya dengan command `./kalkulator -operasi`, berikut saya lampirkan pengerjaannya :

![WhatsApp Image 2024-05-11 at 8 07 26 PM](https://github.com/revalina675/Sisop-3-2024-MH-IT20/assets/150936800/cac24d0e-87c2-4c7c-9bf6-363e5419c18d)

Setelah itu, pada soal diperintahkan untuk menampilkan log dengan nama `race.log`. Disini saya menggunakan command 
```
cat histori.log
```
untuk menampilkannya.
![WhatsApp Image 2024-05-11 at 8 10 50 PM](https://github.com/revalina675/Sisop-3-2024-MH-IT20/assets/150936800/d02062ba-e063-4b45-a694-8933c313966e)

4 line terbawah dari hasil log tersebut adalah bukti untuk tampilan file.log pada dokumentasi sebelumnya.

## Soal 3

Shall Leglerg🥶 dan Carloss Signs 😎 adalah seorang pembalap F1 untuk tim Ferrari 🥵. Mobil F1 memiliki banyak pengaturan, seperti penghematan ERS, Fuel, Tire Wear dan lainnya. Pada minggu ini ada race di sirkuit Silverstone. Malangnya, seluruh tim Ferrari diracun oleh Super Max Max pada hari sabtu sehingga seluruh kru tim Ferrari tidak bisa membantu Shall Leglerg🥶 dan Carloss Signs 😎 dalam race. Namun, kru Ferrari telah menyiapkan program yang bisa membantu mereka dalam menyelesaikan race secara optimal. Program yang dibuat bisa mengatur pengaturan - pengaturan dalam mobil F1 yang digunakan dalam balapan. Programnya ber ketentuan sebagai berikut:

a. Pada program actions.c, program akan berisi function function yang bisa di call oleh paddock.c

b. Action berisikan sebagai berikut:

- Gap [Jarak dengan driver di depan (float)]: Jika Jarak antara Driver dengan Musuh di depan adalah < 3.5s maka return Gogogo, jika jarak > 3.5s dan 10s return Push, dan jika jarak > 10s maka return Stay out of trouble.
- Fuel [Sisa Bensin% (string/int/float)]: Jika bensin lebih dari 80% maka return Push Push Push, jika bensin di antara 50% dan 80% maka return You can go, dan jika bensin kurang dari 50% return Conserve Fuel.
- Tire [Sisa Ban (int)]: Jika pemakaian ban lebih dari 80 maka return Go Push Go Push, jika pemakaian ban diantara 50 dan 80 return Good Tire Wear, jika pemakaian di antara 30 dan 50 return Conserve Your Tire, dan jika pemakaian ban kurang dari 30 maka return Box Box Box.
- Tire Change [Tipe ban saat ini (string)]: Jika tipe ban adalah Soft return Mediums Ready, dan jika tipe ban Medium return Box for Softs.

Contoh:

[Driver] : [Fuel] [55%]
[Paddock]: [You can go]

c. Pada paddock.c program berjalan secara daemon di background, bisa terhubung dengan driver.c melalui socket RPC.

d. Program paddock.c dapat call function yang berada di dalam actions.c.

e. Program paddock.c tidak keluar/terminate saat terjadi error dan akan log semua percakapan antara paddock.c dan driver.c di dalam file race.log

Format log:

[Source] [DD/MM/YY hh:mm:ss]: [Command] [Additional-info]

ex :

[Driver] [07/04/2024 08:34:50]: [Fuel] [55%]

[Paddock] [07/04/2024 08:34:51]: [Fuel] [You can go]

f. Program driver.c bisa terhubung dengan paddock.c dan bisa mengirimkan pesan dan menerima pesan serta menampilan pesan tersebut dari paddock.c sesuai dengan perintah atau function call yang diberikan.

g. Jika bisa digunakan antar device/os (non local) akan diberi nilai tambahan.

h. untuk mengaktifkan RPC call dari driver.c, bisa digunakan in-program CLI atau Argv (bebas) yang penting bisa send command seperti poin B dan menampilkan balasan dari paddock.c
		
ex:

Argv: 

./driver -c Fuel -i 55% 

in-program CLI:

Command: Fuel

Info: 55%


Contoh direktori 😶‍🌫️:

.		.

├── client

│   └── driver.c

└── server

  ├── actions.c
    
  ├── paddock.c
    
  └── race.log

## Jawab

Pada soal tersebut, awalnya kita disuruh untuk membuat 3 program, yaitu `actions.c`, `paddock.c`, dan `driver.c`.

Dimana `paddock.` adalah server, dan `driver.c` adalah client.

Berikut saya lampiran konfigurasi code untuk program-program tersebut

*actions.c*

```
#include <stdio.h>
#include <string.h>

char* Gap(float distance) {
    if (distance < 3.5)
        return "Gogogo";
    else if (distance >= 3.5 && distance <= 10)
        return "Push";
    else
        return "Stay out of trouble";
}

char* Fuel(int percent) {
    if (percent > 80)
        return "Push Push Push";
    else if (percent >= 50 && percent <= 80)
        return "You can go";
    else
        return "Conserve Fuel";
}

char* Tire(int wear) {
    if (wear > 80)
        return "Go Push Go Push";
    else if (wear >= 50 && wear <= 80)
        return "Good Tire Wear";
    else if (wear >= 30 && wear < 50)
        return "Conserve Your Tire";
    else
        return "Box Box Box";
}

char* TireChange(char* current_type) {
    if (strcmp(current_type, "Soft") == 0)
        return "Mediums Ready";
    else if (strcmp(current_type, "Medium") == 0)
        return "Box for Softs";
    else
        return "Unknown tire type";
}
```

`Gap` : mengembalikan saran kepada pembalap berdasarkan jarak yang diukur. Jika jarak kurang dari 3.5, saran adalah "Gogogo", jika antara 3.5 dan 10, saran adalah "Push", dan jika lebih dari 10, saran adalah "Stay out of trouble".

`Fuel ` : memberikan saran kepada pembalap berdasarkan persentase bahan bakar yang tersisa. Jika lebih dari 80%, saran adalah "Push Push Push", jika antara 50% dan 80%, saran adalah "You can go", dan jika kurang dari 50%, saran adalah "Conserve Fuel".

`Tire` : memberikan saran kepada pembalap berdasarkan keausan ban. Jika lebih dari 80%, saran adalah "Go Push Go Push", jika antara 50% dan 80%, saran adalah "Good Tire Wear", jika antara 30% dan kurang dari 50%, saran adalah "Conserve Your Tire", dan jika kurang dari 30%, saran adalah "Box Box Box".

`TireChange` : memberikan saran kepada tim balap tentang pergantian jenis ban yang tepat berdasarkan jenis ban yang saat ini digunakan. Jika ban saat ini adalah "Soft", saran adalah "Mediums Ready", jika "Medium", saran adalah "Box for Softs", dan jika jenis ban tidak dikenali, saran adalah "Unknown tire type".

*paddock.c*

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>
#include "actions.c"

#define PORT 8080

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void daemonize() {
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void handle_sigchld(int sig) {
    // Do nothing to avoid zombie processes
}

int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];

    daemonize();

    // Setup signal handler for SIGCHLD
    signal(SIGCHLD, handle_sigchld);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        pid_t pid = fork();
        if (pid == 0) {  // Child process
            close(sockfd);

            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0) error("ERROR reading from socket");
            printf("Received message: %s\n", buffer);

            char *token = strtok(buffer, " ");
            char *command = token;
            token = strtok(NULL, " ");
            char *value = token;

            // Process command
            char* response;
            if (strcmp(command, "Gap") == 0)
                response = Gap(atof(value));
            else if (strcmp(command, "Fuel") == 0)
                response = Fuel(atoi(value));
            else if (strcmp(command, "Tire") == 0)
                response = Tire(atoi(value));
            else if (strcmp(command, "TireChange") == 0)
                response = TireChange(value);
            else
                response = "Invalid command";

            // Send response
            n = write(newsockfd, response, strlen(response));
            if (n < 0) error("ERROR writing to socket");

            close(newsockfd);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            error("ERROR on fork");
        } else {
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}
```

`error` : digunakan untuk menampilkan pesan kesalahan dan keluar dari program dengan kode kesalahan.

`daemonize()` : mengubah proses utama menjadi daemon, yaitu proses yang berjalan di latar belakang. Ini melibatkan pembuatan proses baru, mengatur SID, mengubah direktori kerja, serta menutup file descriptor standar.

`handle_sigchld` : untuk penanganan sinyal untuk SIGCHLD. Tujuannya adalah untuk mencegah proses menjadi proses zombie setelah selesai.

`main` : merupakan fungsi utama program, dimana dilakukan inisialisasi socket, melakukan binding, dan mendengarkan permintaan koneksi dari klien. Ketika koneksi diterima, server akan membuat proses anak untuk menangani koneksi tersebut. Proses anak akan membaca permintaan dari klien, memprosesnya, dan mengirimkan respons kembali ke klien. Proses induk akan melanjutkan menerima koneksi dari klien lainnya.

*driver.c*

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[1], "-c") != 0 || strcmp(argv[3], "-i") != 0) {
        fprintf(stderr, "Usage: %s -c [Command] -i [Value]\n", argv[0]);
        exit(1);
    }

    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        error("ERROR invalid server address");

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Construct message
    char message[256];
    snprintf(message, sizeof(message), "%s %s", argv[2], argv[4]);

    // Send message to paddock
    n = write(sockfd, message, strlen(message));
    if (n < 0)
        error("ERROR writing to socket");

    // Read response from paddock
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Response from paddock: %s\n", buffer);

    close(sockfd);
    return 0;
}
```

`error` : igunakan untuk menampilkan pesan kesalahan dan keluar dari program dengan kode kesalahan.

`main` : merupakan fungsi utama program, dimana fungsi ini memeriksa argumen baris perintah untuk memastikan format yang benar. Membuat socket, mengatur alamat server, dan melakukan koneksi ke server. Kemudian, membangun pesan yang akan dikirimkan ke server berdasarkan argumen baris perintah, mengirim pesan tersebut ke server, dan membaca respons dari server. Setelah itu, menutup koneksi dan keluar dari program.

Setelah itu saya mengexcecuteable program `paddock.c` dengan mengggunakan command :
```
gcc paddock.c -o paddock
```

Dan juga menjalankan program tersebut dengan command :

```
./paddock
```

Nah, setelah itu kita bisa menjalankan program sesuai dengan masing-masing case yang diminta soal. Berikut saya sertakan dokumentasi untuk beberapa contoh pengerjaannya.

![WhatsApp Image 2024-05-11 at 8 32 21 PM](https://github.com/revalina675/Sisop-3-2024-MH-IT20/assets/150936800/f00adde6-ab68-47e9-a336-9015a97cc8a8)

Setelah itu, pada soal juga diperintahkan untuk menampilkan file.log yang dengan nama 'race.log'. Disini saya menggunakan command `cat race.log` untuk menampilkannya pada terminal.

![WhatsApp Image 2024-05-11 at 8 34 51 PM](https://github.com/revalina675/Sisop-3-2024-MH-IT20/assets/150936800/8974af38-c762-4804-9615-d03bddd89610)

4 baris terbawa adalah bukti untuk tampilan log pada dokumentasi sebelumnya.


## Soal 4

Lewis Hamilton 🏎 seorang wibu akut dan sering melewatkan beberapa episode yang karena sibuk menjadi asisten. Maka dari itu dia membuat list anime yang sedang ongoing (biar tidak lupa) dan yang completed (anime lama tapi pengen ditonton aja). Tapi setelah Lewis pikir-pikir malah kepikiran untuk membuat list anime. Jadi dia membuat file (harap diunduh) dan ingin menggunakan socket yang baru saja dipelajarinya untuk melakukan CRUD pada list animenya. 

a. Client dan server terhubung melalui socket. 

b. client.c di dalam folder client dan server.c di dalam folder server

c. Client berfungsi sebagai pengirim pesan dan dapat menerima pesan dari server.

d. Server berfungsi sebagai penerima pesan dari client dan hanya menampilkan pesan perintah client saja.  

e. Server digunakan untuk membaca myanimelist.csv. Dimana terjadi pengiriman data antara client ke server dan server ke client.

- Menampilkan seluruh judul
- Menampilkan judul berdasarkan genre
- Menampilkan judul berdasarkan hari
- Menampilkan status berdasarkan berdasarkan judul
- Menambahkan anime ke dalam file myanimelist.csv
- Melakukan edit anime berdasarkan judul
- Melakukan delete berdasarkan judul
- Selain command yang diberikan akan menampilkan tulisan “Invalid Command”

f. Karena Lewis juga ingin track anime yang ditambah, diubah, dan dihapus. Maka dia membuat server dapat mencatat anime yang dihapus dalam sebuah log yang diberi nama change.log.

- Format: [date] [type] [massage]
- Type: ADD, EDIT, DEL
- Ex:
  1. [29/03/24] [ADD] Kanokari ditambahkan.
  2. [29/03/24] [EDIT] Kamis,Comedy,Kanokari,completed diubah menjadi Jumat,Action,Naruto,completed.
  3. [29/03/24] [DEL] Naruto berhasil dihapus.

g. Koneksi antara client dan server tidak akan terputus jika ada kesalahan input dari client, cuma terputus jika user mengirim pesan “exit”. Program exit dilakukan pada sisi client.

h. Hasil akhir:

soal_4/

.
    ├── change.log
.    
    ├── client/
.    
    │   └── client.c
.    
    ├── myanimelist.csv
.    
    └── server/
.    
        └── server.c

## Jawab

Langkah pertama adalah mendownload file `myanimelist.csv` pada link Google Drive yang tersedia dengan command
```
wget https://drive.google.com/file/d/10p_kzuOgaFY3WT6FVPJIXFbkej2s9f50/view?usp=drive_link
```

Berikut ini adalah isi dari file `myanimelist.csv`

[gambar isi file csv]
