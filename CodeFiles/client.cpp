#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef std::string string;
typedef sockaddr_in SocketAddressIn;

extern void Print(bool isServer, string txt);

void StartClient() {
    int sock = 0, valread;
    SocketAddressIn serv_addr;
    char hello[] = "Hello from client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    send(sock, hello, strlen(hello), 0);
    Print(false, "Hello message sent");
    valread = read(sock, buffer, 1024);
    Print(false, "Received: " + (string)buffer );

    close(sock);
}
