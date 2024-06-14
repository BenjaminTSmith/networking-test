#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#elif _WIN32
#include <windows.h>
#endif

#include <iostream>

int main (int argc, char** argv) {
    std::cout << "Hello, World!\n";
    return 0;
}
