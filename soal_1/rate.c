#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARED_MEM_KEY 1234
#define MAX_FILENAME_LENGTH 256

// Struktur untuk menyimpan data tempat sampah atau parkiran
struct Place {
    char name[MAX_FILENAME_LENGTH];
    float rating;
};

// Fungsi untuk mencari tempat dengan rating tertinggi
struct Place findBestPlace(FILE *file) {
    struct Place bestPlace = {"", 0.0};
    // Melewati baris header
    char line[MAX_FILENAME_LENGTH];
    fgets(line, sizeof(line), file); // Membaca dan membuang baris header

    // Membaca data dari file CSV
    while (fgets(line, sizeof(line), file) != NULL) {
        // Memisahkan nama tempat dan rating
        char name[MAX_FILENAME_LENGTH];
        float rating;
        sscanf(line, "%[^,],%f", name, &rating);

        // Memilih tempat dengan rating terbaik
        if (rating > bestPlace.rating) {
            strcpy(bestPlace.name, name);
            bestPlace.rating = rating;
        }
    }
    return bestPlace;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.csv>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    // Membaca nama file dari shared memory
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, argv[1]);

    // Menampilkan output
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

    return 0;
}
