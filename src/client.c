#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: ./client ip port\n");
        exit(0);
    }
    struct addrinfo hints;
    struct addrinfo* results;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(argv[1], argv[2], &hints, &results);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(0);
    }

    int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (sockfd == -1) {
        perror("socket error");
        exit(0);
    }

    status = connect(sockfd, results->ai_addr, results->ai_addrlen);
    if (status == -1) {
        perror("connect error");
        exit(0);
    }

    printf("I DID IT!\n");

    freeaddrinfo(results);
    return 0;
}
