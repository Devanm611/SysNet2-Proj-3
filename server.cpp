// server_verbose.cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <map>
#include <sstream>
#include <set>
#include "User.hpp"

std::map<std::string, User> onlineUsers;
std::map<std::string, std::set<std::string>> userLocations; // Map to store user locations
std::mutex userMutex;

void handleClient(int clientSocket){
    std::cout << "[+] New client connected: socket " << clientSocket << std::endl;
    char buffer[1024];
    std::string username;

    while(true){
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(bytesReceived <= 0){
            std::cerr << "[-] Client disconnected or error occurred on socket " << clientSocket << std::endl;
            break;
        }

        std::string input(buffer);
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        std::cout << "[>] Received from client (" << clientSocket << "): " << input << std::endl;

        if(command == "Register"){
            std::string uname, pwd;
            iss >> uname >> pwd;

            std::cout << "[*] Handling REGISTER for user: " << uname << std::endl;
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
            if(exists){
                msg = "ERROR: Username already exists\n";
                std::cout << "[-] Registration failed: Username exists\n";
            } else {
                std::ofstream outfile("users.txt", std::ios::app);
                outfile << uname << " " << pwd << std::endl;
                msg = "SUCCESS: Account created\n";
                std::cout << "[+] Registration successful\n";
            }

            std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
            send(clientSocket, msg.c_str(), msg.size(), 0);
        }

        else if(command == "Login"){
            std::string uname, pwd;
            iss >> uname >> pwd;

            std::cout << "[*] Handling LOGIN for user: " << uname << std::endl;
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
            if(success){
                username = uname;
                onlineUsers[username] = User(uname, pwd, clientSocket);
                msg = "SUCCESS\n";
                std::cout << "[+] Login SUCCESS for user: " << uname << std::endl;
            } else {
                msg = "ERROR\n";
                std::cout << "[-] Login FAILED for user: " << uname << std::endl;
            }

            std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
            send(clientSocket, msg.c_str(), msg.size(), 0);
        }

        else if(command == "ChangePassword") {
            std::string uname, oldpass, newpass;
            iss >> uname >> oldpass >> newpass;

            std::cout << "[*] Handling PASSWORD CHANGE for user: " << uname << std::endl;
            std::lock_guard<std::mutex> lock(userMutex);
            std::ifstream infile("users.txt");
            std::vector<std::pair<std::string, std::string>> users;
            std::string u, p;
            bool updated = false;

            while (infile >> u >> p) {
                if (u == uname && p == oldpass) {
                    users.emplace_back(u, newpass);
                    updated = true;
                } else {
                    users.emplace_back(u, p);
                }
            }
            infile.close();

            std::ofstream outfile("users.txt", std::ios::trunc);
            for (const auto& user : users) {
                outfile << user.first << " " << user.second << "\n";
            }

            std::string msg;
            if (updated) {
                if (onlineUsers.find(uname) != onlineUsers.end()) {
                    onlineUsers[uname].setPassword(newpass);
                }
                msg = "SUCCESS: Password updated\n";
                std::cout << "[+] Password changed successfully\n";
            } else {
                msg = "ERROR: Invalid credentials\n";
                std::cout << "[-] Password change failed\n";
            }

            std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
            send(clientSocket, msg.c_str(), msg.size(), 0);
        }

        else if(command == "Subscribe"){
            std::string location;
            std::getline(iss >> std::ws, location);

            std::cout << "[*] Handling SUBSCRIBE for " << username << " to " << location << std::endl;
            std::lock_guard<std::mutex> lock(userMutex);

            if (!location.empty()) {
                userLocations[username].insert(location); // Store location in memory
                std::string msg = "SUCCESS: Subscribed to " + location + "\n";
                std::cout << "[+] Subscribed successfully to " << location << std::endl;
                std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
                send(clientSocket, msg.c_str(), msg.size(), 0);
            } else {
                std::string msg = "ERROR: Invalid location\n";
                std::cout << "[-] Subscription failed: Invalid location\n";
                std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
        }

        else if(command == "Unsubscribe"){
            std::string location;
            std::getline(iss >> std::ws, location);

            std::cout << "[*] Handling UNSUBSCRIBE for " << username << " from " << location << std::endl;
            std::lock_guard<std::mutex> lock(userMutex);

            if (userLocations[username].erase(location)) { // Remove location from memory
                std::string msg = "SUCCESS: Unsubscribed from " + location + "\n";
                std::cout << "[+] Unsubscribed successfully from " << location << std::endl;
                std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
                send(clientSocket, msg.c_str(), msg.size(), 0);
            } else {
                std::string msg = "ERROR: Not subscribed to " + location + "\n";
                std::cout << "[-] Unsubscription failed: Not subscribed to " << location << std::endl;
                std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
        }

        else if(command == "Subscriptions"){
            std::lock_guard<std::mutex> lock(userMutex);
            std::set<std::string> subs = userLocations[username];
            std::string msg;

            std::cout << "[*] Fetching subscriptions for user: " << username << std::endl;
            if(subs.empty()){
                msg = "No subscriptions found.\n";
            } else {
                msg = "Your current subscriptions are:\n";
                for(const auto& loc : subs){
                    msg += loc + "\n";
                }
            }

            std::cout << "[<] Sending to client (" << clientSocket << "): " << msg;
            send(clientSocket, msg.c_str(), msg.size(), 0);
        }

        else if(command == "Exit"){
            std::lock_guard<std::mutex> lock(userMutex);
            onlineUsers.erase(username);
            userLocations.erase(username); // Remove user locations from memory
            std::cout << "[-] User " << username << " logged out and removed from online list.\n";
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
    address.sin_port = htons(60500);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "[✓] Server running on port 60500..." << std::endl;

    while(true){
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        std::cout << "[*] Accepted new connection on socket " << new_socket << std::endl;
        std::thread t(handleClient, new_socket);
        t.detach();
    }

    return 0;
}
