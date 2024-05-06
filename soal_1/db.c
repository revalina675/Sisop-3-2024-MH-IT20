#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define SHARED_MEM_KEY 1234
#define MAX_FILENAME_LENGTH 256
#define LOG_FILE "database/db.log"

// Deklarasi struktur FileData di sini
struct FileData {
    char filename[MAX_FILENAME_LENGTH];
};

// Fungsi untuk menyalin nama file ke dalam shared memory
void copyFilenameFromSharedMemory(char *shm_ptr, char *filename) {
    strcpy(filename, shm_ptr);
}

// Fungsi untuk memindahkan file ke folder microservices/database
void moveFile(char *filename) {
    char oldpath[MAX_FILENAME_LENGTH + 20]; // Menyertakan path folder "new-data"
    sprintf(oldpath, "../new-data/%s", filename);
    char newpath[MAX_FILENAME_LENGTH + 30]; // Menyertakan path folder "microservices/database"
    sprintf(newpath, "./database/%s", filename);

    // Membuat direktori "microservices/database" jika belum ada
    mkdir("./database", 0777);

    if (rename(oldpath, newpath) == 0) {
        printf("File %s berhasil dipindahkan ke microservices/database.\n", filename);
    } else {
        perror("rename");
    }
}

// Fungsi untuk menentukan tipe file berdasarkan nama file
char* determineFileType(char *filename) {
    if (strstr(filename, "trashcan") != NULL) {
        return "Trash Can";
    } else if (strstr(filename, "parkinglot") != NULL) {
        return "Parking Lot";
    } else {
        return "Unknown";
    }
}

// Fungsi untuk menuliskan log ke file db.log
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

int main() {
    // Membaca shared memory yang telah dibuat oleh auth.c
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

    // Membaca nama file dari shared memory dan memindahkannya
    char filename[MAX_FILENAME_LENGTH];
    copyFilenameFromSharedMemory(shm_ptr, filename);
    moveFile(filename);
    char *fileType = determineFileType(filename);
    writeLog(fileType, filename);

    // Menyalin semua file yang terautentikasi dari folder new-data ke database
    DIR *dir = opendir("../new-data");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Hanya memproses file biasa (bukan direktori)
            char filename[MAX_FILENAME_LENGTH];
            strcpy(filename, entry->d_name);
            char *fileType = determineFileType(filename);
            if (strcmp(fileType, "Trash Can") == 0 || strcmp(fileType, "Parking Lot") == 0) {
                moveFile(filename);
                writeLog(fileType, filename);
            }
        }
    }

    closedir(dir);

    // Melepaskan shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
