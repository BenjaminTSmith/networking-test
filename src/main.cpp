#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

#include <iostream>

#define DEFAULT_PORT "27015"

int main (int argc, char** argv) {
    WSADATA wsa_data;
    int i_result;
    i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0) {
        std::cout << "WSAStartup failed: " << i_result << "\n";
        return 1;
    }

    struct addrinfo* result = nullptr, * ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    i_result = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
    if (i_result != 0) {
        std::cout << "getaddrinfo failed: " << i_result << "\n";
        WSACleanup();
        return 1;
    }

    SOCKET listen_socket = INVALID_SOCKET;
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    i_result = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
    if (i_result == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);


    return 0;
}
