#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080             //menentukan port yang digunakan oleh client
#define MAX_BUFFER_SIZE 1024

int main()
{
    int sock = 0, bt_read;          //sock untuk handle file socket, bt_read untuk simpan byt>
    struct sockadd_in serv_add;        //simpan alamat server pada serv_add
    char buffer[MAX_BUFFER_SIZE] = {0};
    //membuat socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket gagal, berakhir dengan kesalahan \n");
        return -1;
    }
    serv_add.sin_family = AF_INET;       //mengisi informasi dengan alamat server dan port
    serv_add.sin_port = htons(PORT);
    //convert alamat IPv4 dan alamat IP dari string menjadi biner
    if(inet_pton(AF_INET, "127.0.0.1", &serv_add.sin_add) <= 0) {
        printf("\nAddress tidak valid \n");
        return -1;
    }
    //program membuat koneksi dengan server
    if (connect(sock, (struct sockadd *)&serv_add, sizeof(serv_add)) < 0) {
        printf("\nKoneksi dengan server gagal \n");
        return -1;
    }
    //data diterima dari server
    bt_read = read(sock, buffer, MAX_BUFFER_SIZE);
    printf("%s\n", buffer);
    return 0;
}
