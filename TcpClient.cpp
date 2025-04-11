// TcpClient.cpp
#include "TcpClient.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

TcpClient::TcpClient() : sockfd(-1) {}

TcpClient::~TcpClient() {
    if (sockfd != -1) {
        close(sockfd);
    }
}

bool TcpClient::connectToServer(const std::string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::perror("Socket creation failed");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::perror("Connection failed");
        return false;
    }

    return true;
}

bool TcpClient::sendData(const std::string& data) {
    ssize_t bytesSent = send(sockfd, data.c_str(), data.size(), 0);
    if (bytesSent < 0) {
        std::perror("Failed to send data");
        return false;
    }

    if (static_cast<size_t>(bytesSent) != data.size()) {
        std::cerr << "Warning: Not all data was sent. Sent "
                  << bytesSent << " bytes out of " << data.size() << std::endl;
    }

    return true;
}

std::string TcpClient::receiveData() {
    char buffer[1024] = {0};
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived <= 0) {
        std::perror("Failed to receive data");
        return "";
    }

    return std::string(buffer);
}

int TcpClient::getSocket() const {
    return sockfd;
}
