//Server.cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <fstream>
#include <mutex>
#include <map>
#include <sstream>

#include "User.hpp"

std::map<std::string, User> onlineUsers;
std::mutex userMutex;

void handleClient(int clientSocket) {
    char buffer[1024];
    std::string username;

    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected or error occurred" << std::endl;
            break;
        }

        std::string input(buffer);
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "Rigester") {
            std::string uname, pwd;
            iss >> uname >> pwd;

            std::lock_guard<std::mutex> lock(userMutex);
            std::ifstream infile("users.txt");
            std::string line, u, p;
            bool exists = false;
            while (std::getline(infile, line)) {
                std::istringstream lss(line);
                lss >> u >> p;
                if (u == uname) {
                    exists = true;
                    break;
                }
            }
            infile.close();

            if (exists) {
                std::string msg = "ERROR: Username already exists\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            } else {
                std::ofstream outfile("users.txt", std::ios::app);
                outfile << uname << " " << pwd << std::endl;
                std::string msg = "SUCCESS: Account created\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
        }

        else if (command == "Login") {
            std::string uname, pwd;
            iss >> uname >> pwd;

            std::lock_guard<std::mutex> lock(userMutex);
            std::ifstream infile("users.txt");
            std::string u, p;
            bool success = false;
            while (infile >> u >> p) {
                if (u == uname && p == pwd) {
                    success = true;
                    break;
                }
            }
            infile.close();

            if (success) {
                username = uname;
                onlineUsers[username] = User(uname, pwd, clientSocket);
                std::string msg = "SUCCESS: Logged in\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            } else {
                std::string msg = "ERROR: Invalid credentials\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
        }

        else if (command == "Exit") {
            std::lock_guard<std::mutex> lock(userMutex);
            onlineUsers.erase(username);
            break;
        }
    }

    close(clientSocket);
}

int main() {
    int server_fd, new_socket;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server running on port 8080..." << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        std::thread t(handleClient, new_socket);
        t.detach(); // Let threads run independently
    }

    return 0;
}