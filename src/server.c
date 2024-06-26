#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#endif


#define BACKLOG 10

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: ./server port\n");
        exit(1);
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
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // Check for IPv4 address
        if (ifa->ifa_addr->sa_family == AF_INET) {
            int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);
        }
    }
    freeifaddrs(ifaddr);

    struct addrinfo hints;
    struct addrinfo* results;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    status = getaddrinfo(NULL, argv[1], &hints, &results);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(0);
    }

    int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (sockfd == -1) {
        perror("socket error");
        exit(0);
    }

    status = bind(sockfd, results->ai_addr, results->ai_addrlen);
    if (status == -1) {
        perror("bind error");
        exit(0);
    }

    status = listen(sockfd, BACKLOG);
    if (status == -1) {
        perror("listen error");
        exit(0);
    }

    printf("listening on port %s\n", argv[1]);

    struct sockaddr_storage connecting_addr;
    socklen_t addr_size = sizeof(connecting_addr);
    int connecting_sockfd = accept(sockfd, (struct sockaddr*)&connecting_addr, &addr_size);
    if (connecting_sockfd == -1) {
        perror("accept error");
        exit(0);
    }
    struct sockaddr_in* connected_addr = (struct sockaddr_in*)&connecting_addr;

    freeaddrinfo(results);
    return 0;
}
