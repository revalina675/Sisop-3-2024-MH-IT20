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
