#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: ./client ip port\n");
        exit(0);
    }

#ifdef _WIN32
    WSADATA wsa_data;
    int i_result;

    i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0) {
        printf("WSAStartup failed: %d\n", i_result);
        return 1;
    }
#endif

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
