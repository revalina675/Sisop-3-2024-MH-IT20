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

