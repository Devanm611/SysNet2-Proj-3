#include "TcpClient.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

TcpClient::TcpClient(const std::string& ip, int port)
    : serverIP(ip), serverPort(port), sockfd(-1) {}

TcpClient::~TcpClient() {
    closeConnection();
}

bool TcpClient::connectToServer() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed.\n";
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address.\n";
        return false;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed.\n";
        return false;
    }

    return true;
}

bool TcpClient::sendData(const std::string& data) {
    if (send(sockfd, data.c_str(), data.length(), 0) < 0) {
        std::cerr << "Send failed.\n";
        return false;
    }
    return true;
}

std::string TcpClient::receiveData() {
    char buffer[1024] = {0};
    int bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        return "";
    }
    return std::string(buffer, bytesRead);
}

void TcpClient::closeConnection() {
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
    }
}

int TcpClient::getSocket() const {
    return sockfd;
}
