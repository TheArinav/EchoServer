#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

typedef sockaddr_in SocketAddressIn;
typedef std::string string;

extern void Print(bool isServer, string txt);

int server_fd;

bool BuildSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(server_fd);
        return false;
    }
    return true;
}

SocketAddressIn BindSocket() {
    SocketAddressIn addressIn;
    addressIn.sin_family = AF_INET;
    addressIn.sin_addr.s_addr = INADDR_ANY;
    addressIn.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&addressIn, sizeof(addressIn)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        throw std::runtime_error("Bind failed");
    }
    return addressIn;
}

void StartServer() {
    if (BuildSocket()) {
        try {
            SocketAddressIn address = BindSocket();

            if (listen(server_fd, 3) < 0) {
                std::cerr << "Listen failed" << std::endl;
                close(server_fd);
                return;
            }

            int addressLen = sizeof(address);
            int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addressLen);
            if (new_socket < 0) {
                std::cerr << "Accept failed" << std::endl;
                close(server_fd);
                return;
            }

            char buffer[1024] = {0};
            int valRead = read(new_socket, buffer, 1024);
            Print(true, "Received: "+ (string)buffer);
            send(new_socket, buffer, valRead, 0);
            Print(true, "Echoed message back");

            close(new_socket);
            close(server_fd);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
