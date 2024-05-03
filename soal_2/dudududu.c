#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    if (len == 1) {
        strcpy(output, ones[*num - '0']);
        return;
    }
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

void log_history(char *operation, char *num1_str, char *num2_str, char *result_str) {
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

    char upper_operation[10];
    char *operation_name = operation + 1; // Skip the '-' character
    strcpy(upper_operation, operation_name);
    for (int i = 0; upper_operation[i]; i++) {
        upper_operation[i] = toupper(upper_operation[i]);
    }
    fprintf(log_file, "[%s] [%s] hasil %s %s dan %s adalah %s.\n", timestamp, upper_operation, operation_name, num1_str, num2_str, result_str);
    fclose(log_file);
}

int main(int argc, char *argv[]) {
    // Check if argument is provided
    if (argc != 2) {
        printf("Usage: %s <operation>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-kali") != 0 && strcmp(argv[1], "-tambah") != 0 && strcmp(argv[1], "-kurang") != 0 && strcmp(argv[1], "-bagi") != 0) {
        printf("Invalid operation\n");
        return 1;
        
    }
    char input[100];
    printf("Masukkan dua angka (satu sampai sembilan): ");
    fgets(input, sizeof(input), stdin);
    strtok(input, "\n");

    char num1_str[10], num2_str[10];
    sscanf(input, "%s %s", num1_str, num2_str);

    int num1 = convert_to_number(num1_str);
    int num2 = convert_to_number(num2_str);

    if (num1 == -1 || num2 == -1) {
        printf("Invalid input\n");
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { 
        close(pipefd[0]);

        char result_str[100];
        calculate_result(num1, num2, argv[1], result_str);
        char result_words[100];
        convert_to_words(result_str, result_words);
        write(pipefd[1], result_words, strlen(result_words) + 1);
        close(pipefd[1]);

        exit(0);
    } else { 
        close(pipefd[1]);

        char result[100];
        read(pipefd[0], result, sizeof(result));
        close(pipefd[0]);

        log_history(argv[1], num1_str, num2_str, result);
        printf("\"Hasil %s %s dan %s adalah %s.\"\n", argv[1], num1_str, num2_str, result);
        
        return 0;
    }
}

