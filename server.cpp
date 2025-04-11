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
    std::cout << "[+] New client connected: socket " << clientSocket << std::endl;

    char buffer[1024];
    std::string username;

    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "[-] Client disconnected or error occurred on socket " << clientSocket << std::endl;
            break;
        }

        std::string input(buffer);
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        std::cout << "[>] Received from client " << clientSocket << ": " << input << std::endl;

        if (command == "Register") {
            std::string uname, pwd;
            iss >> uname >> pwd;
            std::cout << "[*] Handling registration for user: " << uname << std::endl;

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

            std::string msg;
            if (exists) {
                msg = "ERROR: Username already exists\n";
                std::cout << "[-] Registration failed: " << uname << " already exists\n";
            } else {
                std::ofstream outfile("users.txt", std::ios::app);
                outfile << uname << " " << pwd << std::endl;
                msg = "SUCCESS: Account created\n";
                std::cout << "[+] Registration successful for user: " << uname << std::endl;
            }
            send(clientSocket, msg.c_str(), msg.size(), 0);
            std::cout << "[<] Sent to client: " << msg;
        }

        else if (command == "Login") {
            std::string uname, pwd;
            iss >> uname >> pwd;
            std::cout << "[*] Attempting login for user: " << uname << std::endl;

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

            std::string msg;
            if (success) {
                username = uname;
                onlineUsers[username] = User(uname, pwd, clientSocket);
                msg = "SUCCESS: Logged in\n";
                std::cout << "[+] Login successful for user: " << uname << std::endl;
            } else {
                msg = "ERROR: Invalid credentials\n";
                std::cout << "[-] Login failed for user: " << uname << std::endl;
            }
            send(clientSocket, msg.c_str(), msg.size(), 0);
            std::cout << "[<] Sent to client: " << msg;
        }

        else if(command == "Subscribe"){
            std::string location;
            iss >> location;
            std::cout << "[*] Handling subscription for user: " << username << " to valid location: " << location << std::endl;
            
            std::lock_guard<std::mutex> lock(userMutex);
            bool success = false;

            //Check if the location is valid
            if(location == "Pensacola" || location == "Destin" || location == "Fort Walton Beach" || location == "Crestview" || location == "Navarre"){
                success = true;
                onlineUsers[username].subscribeToLocation(location);
            }

            std::string msg;
            if(success){
                msg = "SUCCESS: Subscribed to " + location + "\n";
                std::cout << "[+] Subscription successful for user: " << username << " to location: " << location << std::endl;
            } 
            else{
                msg = "ERROR: Invalid location\n";
                std::cout << "[-] Subscription failed for user: " << username << " to location: " << location << std::endl;
            }

            send(clientSocket, msg.c_str(), msg.size(), 0);
            std::cout << "[<] Sent to client: " << msg;
        }
        //else if(command == "Unsubscribe"){
            //std::lock_guard<std::mutex> lock(userMutex);
        //}

        else if (command == "Exit") {
            std::lock_guard<std::mutex> lock(userMutex);
            onlineUsers.erase(username);
            std::cout << "[-] User " << username << " disconnected and removed from online list.\n";
            break;
        }
    }

    close(clientSocket);
    std::cout << "[x] Closed connection for socket " << clientSocket << std::endl;
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

    std::cout << "[✓] Server running on port 8080..." << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        std::cout << "[*] Accepted new connection on socket " << new_socket << std::endl;
        std::thread t(handleClient, new_socket);
        t.detach();
    }

    return 0;
}
