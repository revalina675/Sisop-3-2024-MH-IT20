#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 
#include <unistd.h>

#define SHARED_MEM_KEY 1234
#define MAX_FILENAME_LENGTH 256

// Deklarasi struktur FileData di sini
struct FileData {
    char filename[MAX_FILENAME_LENGTH];
};

// Fungsi untuk memeriksa apakah file memenuhi syarat autentikasi
int isFileAuthenticated(char *filename) {
    // Memeriksa apakah nama file berakhir dengan "trashcan.csv" atau "parkinglot.csv"
    int len = strlen(filename);
    if (len >= 12 && strcmp(filename + len - 12, "trashcan.csv") == 0) {
        return 1; // File lolos autentikasi
    } else if (len >= 14 && strcmp(filename + len - 14, "parkinglot.csv") == 0) {
        return 1; // File lolos autentikasi
    }
    return 0; // File tidak lolos autentikasi
}

// Fungsi untuk menyalin nama file ke dalam shared memory
void copyFilenameToSharedMemory(char *shm_ptr, char *filename) {
    strcpy(shm_ptr, filename);
}

// Fungsi thread untuk memproses file
void *processFile(void *arg) {
    struct FileData *fileData = (struct FileData *)arg;
    printf("Processing file: %s\n", fileData->filename);
    // Tambahkan logika di sini untuk menangani file
    // Sebagai contoh, kita hanya mencetak nama file
    return NULL;
}

// Fungsi untuk menghapus file
void deleteFile(char *filepath) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Proses anak
        if (remove(filepath) == 0) {
            printf("File %s tidak memenuhi syarat autentikasi dan telah dihapus.\n", filepath);
        } else {
            perror("remove");
        }
        exit(EXIT_SUCCESS);
    } else { // Proses induk
        wait(NULL); // Memanggil wait di sini
    }
}

int main() {
    // Membuat shared memory
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

    // Mencari file yang memenuhi syarat autentikasi di folder "new-data"
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir("new-data")) != NULL) {
        pthread_t tid;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) { // Hanya memproses file biasa (bukan direktori)
                char filename[MAX_FILENAME_LENGTH];
                strcpy(filename, entry->d_name);
                if (isFileAuthenticated(filename)) {
                    // Menyalin nama file ke shared memory
                    copyFilenameToSharedMemory(shm_ptr, filename);
                    printf("File %s telah terautentikasi dan disalin ke shared memory.\n", filename);
                    // Membuat thread untuk memproses file
                    struct FileData fileData; // Mendeklarasikan fileData di dalam loop while
                    strcpy(fileData.filename, filename);
                    if (pthread_create(&tid, NULL, processFile, (void *)&fileData) != 0) {
                        perror("pthread_create");
                    }
                    if (pthread_join(tid, NULL) != 0) {
                        perror("pthread_join");
                    }
                } else {
                    // Menghapus file yang tidak memenuhi syarat autentikasi
                    char filepath[MAX_FILENAME_LENGTH + 10]; // Menyertakan path folder "new-data"
                    sprintf(filepath, "new-data/%s", filename);
                    deleteFile(filepath);
                }
            }
        }
        closedir(dir);
    } else {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Melepaskan shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
