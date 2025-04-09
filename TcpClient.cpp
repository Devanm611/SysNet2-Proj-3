#include "TcpClient.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

TcpClient::TcpClient() : sockfd(-1) {}

TcpClient::~TcpClient() {
    if (sockfd != -1) close(sockfd);
}

bool TcpClient::connectToServer(const string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return false;
    }

    return true;
}

bool TcpClient::sendData(const string& data) {
    return send(sockfd, data.c_str(), data.size(), 0) >= 0;
}

string TcpClient::receiveData() {
    char buffer[1024] = {0};
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) return "";
    return std::string(buffer);
}

int TcpClient::getSocket() const {
    return sockfd;
}
