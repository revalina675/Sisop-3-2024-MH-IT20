
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
