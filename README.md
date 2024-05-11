# PRAKTIKUM MODUL 03 SISTEM OPERASI
## Kelompok IT20
### Anggota Kelompok :
|             Nama              |     NRP    |
|-------------------------------|------------|
| Revalina Fairuzy Azhari Putri | 5027231001 |
| Farida Qurrotu 'Ayuna         | 5027231015 |
| Gallant Damas Hayuaji         | 5027231037 |

## Soal 1


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
    ├── change.log
    ├── client/
    │   └── client.c
    ├── myanimelist.csv
    └── server/
        └── server.c
