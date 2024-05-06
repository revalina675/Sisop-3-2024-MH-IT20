#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080                //mendefinisikan port yang akan digunakan di server
#define MAX_BUFFER_SIZE 1024    

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);              //menentukan ukuran alamat atau address
    char buffer[MAX_BUFFER_SIZE] = {0};
    char *csv_data = "Menampilkan data dari myanimelist.csv";
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  //membuat file socket
        perror("Gagal membuat socket");                        //jika gagal, maka muncul kesalahan
        exit(EXIT_FAILURE);
}

    address.sin_family = AF_INET;            //address diisi dengan alamat server, IP, dan port yang dipakai
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //mencoba binding/hubungkan socket yang dibuat dengan alamat port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding gagal");             //binding gagal, maka muncul pesan kesalahan
        exit(EXIT_FAILURE);
    }
    //server menerima input yang masuk pada socket
    if (listen(server_fd, 3) < 0) {
        perror("Koneksi gagal");
        exit(EXIT_FAILURE);
    }
    //server menerima koneksi baru dari client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Koneksi ulang gagal");
        exit(EXIT_FAILURE);
    }
    //server mengirim data csv ke client
    send(new_socket, csv_data, strlen(csv_data), 0);
    printf("Data CSV dikirim\n");
    return 0;
}
