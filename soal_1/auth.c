#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define SHARED_MEM_SIZE 4096
#define FILENAME_MAX_LEN 256
#define MAX_FILES 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct SharedMemory {
    char filenames[MAX_FILES][FILENAME_MAX_LEN];
    int count;
};

void *authenticate_and_send_to_shared_memory(void *arg) {
    char *filename = strdup((char *)arg); // Salin filename ke memori baru

    int fd = shm_open("/shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        free(filename); // Bebaskan memori yang dialokasikan
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, SHARED_MEM_SIZE) == -1) {
        perror("ftruncate");
        free(filename); // Bebaskan memori yang dialokasikan
        exit(EXIT_FAILURE);
    }

    struct SharedMemory *shared_memory = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        free(filename); // Bebaskan memori yang dialokasikan
        exit(EXIT_FAILURE);
    }

    // Periksa apakah file memiliki ekstensi .csv dan berakhiran "_trashcan.csv" atau "_parkinglot.csv"
    char *file_ext = strrchr(filename, '.');
    char *file_type = strstr(filename, "_");

    if (file_ext != NULL && strcmp(file_ext, ".csv") == 0 && file_type != NULL && (strcmp(file_type, "_trashcan.csv") == 0 || strcmp(file_type, "_parkinglot.csv") == 0)) {
        pthread_mutex_lock(&mutex);

        // Tambahkan informasi file ke dalam shared memory jika masih ada ruang
        if (shared_memory->count < MAX_FILES) {
            strcpy(shared_memory->filenames[shared_memory->count], filename);
            shared_memory->count++;
        }

        pthread_mutex_unlock(&mutex);

        printf("Authenticated and sent to shared memory: %s\n", filename);
    } else {
        printf("File failed authentication, removing: %s\n", filename);
        // Hapus file yang tidak memenuhi kriteria
        if (remove(filename) == -1) {
            perror("remove");
            free(filename); // Bebaskan memori yang dialokasikan
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd) == -1) {
        perror("close");
        free(filename); // Bebaskan memori yang dialokasikan
        exit(EXIT_FAILURE);
    }

    free(filename); // Bebaskan memori yang dialokasikan
    pthread_exit(NULL);
}

void process_new_data_folder() {
    DIR *dir = opendir("new-data");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    pthread_t tid[MAX_FILES]; // Array untuk menyimpan ID thread
    int thread_count = 0; // Hitung jumlah thread yang telah dibuat
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filename[FILENAME_MAX_LEN];
            snprintf(filename, FILENAME_MAX_LEN, "new-data/%s", entry->d_name);
            pthread_create(&tid[thread_count], NULL, authenticate_and_send_to_shared_memory, strdup(filename));
            thread_count++; // Tambahkan jumlah thread yang telah dibuat
        }
    }

    // Tunggu setiap thread selesai
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tid[i], NULL);
    }

    closedir(dir);
}

int main() {
    process_new_data_folder();
    return 0;
}
