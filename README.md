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
