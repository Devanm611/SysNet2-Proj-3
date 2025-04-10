#include "TcpClient.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

TcpClient::TcpClient() : sockfd(-1) {} // Constructor initializes sockfd to -1

TcpClient::~TcpClient() { // Destructor closes the socket if it's open
    if (sockfd != -1) {
        close(sockfd);
    }
}

bool TcpClient::connectToServer(const string& ip, int port) { 
    // Connects to the server at the specified IP and port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    // Set up the server address structure
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    // Connect to the server
    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return false;
    }

    return true;
}

bool TcpClient::sendData(const string& data) { // Sends data to the server
    
    ssize_t bytesSent = send(sockfd, data.c_str(), data.size(), 0);
    if (bytesSent < 0) {
        perror("Failed to send data");
        return false;
    }
    // Check if all data was sent
    if (static_cast<size_t>(bytesSent) != data.size()) {
        cerr << "Warning: Not all data was sent. Sent " << bytesSent << " bytes out of " << data.size() << endl;
    }
    return true;

    //return send(sockfd, data.c_str(), data.size(), 0) >= 0;
}

string TcpClient::receiveData() {
    // Receives data from the server
    // Create a buffer to store the received data
    char buffer[1024] = {0};
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived <= 0) {
        perror("Failed to receive data");
        return "";
    }

    return string(buffer);
}

int TcpClient::getSocket() const {
    return sockfd;
}
