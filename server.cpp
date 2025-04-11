#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include "User.hpp"

#define PORT 12345

std::mutex userMutex;
std::map<std::string, User> users; // username → User

// Load users from file
void loadUsers() {
    std::ifstream file("user.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password, locString;
        if (!(iss >> username >> password)) continue;
        std::getline(iss, locString);
        User user(username, password, -1);

        std::istringstream locStream(locString);
        std::string loc;
        while (std::getline(locStream, loc, ',')) {
            if (!loc.empty()) user.subscribeLocation(loc);
        }

        users[username] = user;
    }
}

// Save users to file
void saveUsers() {
    std::ofstream file("user.txt");
    for (auto& [username, user] : users) {
        file << username << " " << user.getPassword();
        for (const auto& loc : user.getLocations()) {
            file << "," << loc;
        }
        file << "\n";
    }
}

// Send helper
void sendMsg(int sock, const std::string& msg) {
    send(sock, msg.c_str(), msg.length(), 0);
}

// Receive helper
std::string recvMsg(int sock) {
    char buffer[1024] = {0};
    int len = recv(sock, buffer, sizeof(buffer), 0);
    return (len > 0) ? std::string(buffer, len) : "";
}

void handleClient(int clientSock) {
    std::string username = "";
    bool loggedIn = false;

    sendMsg(clientSock, "Welcome! Choose: LOGIN / REGISTER / QUIT\n");

    while (!loggedIn) {
        std::string cmd = recvMsg(clientSock);
        if (cmd == "LOGIN") {
            sendMsg(clientSock, "Username: ");
            std::string uname = recvMsg(clientSock);
            sendMsg(clientSock, "Password: ");
            std::string pass = recvMsg(clientSock);

            std::lock_guard<std::mutex> lock(userMutex);
            if (users.count(uname) && users[uname].getPassword() == pass) {
                users[uname].setSocketNo(clientSock);
                username = uname;
                loggedIn = true;
                sendMsg(clientSock, "Login successful!\n");
            } else {
                sendMsg(clientSock, "Login failed. Try again.\n");
            }

        } else if (cmd == "REGISTER") {
            sendMsg(clientSock, "Choose a username: ");
            std::string uname = recvMsg(clientSock);
            sendMsg(clientSock, "Choose a password: ");
            std::string pass = recvMsg(clientSock);

            std::lock_guard<std::mutex> lock(userMutex);
            if (users.count(uname)) {
                sendMsg(clientSock, "Username already exists.\n");
            } else {
                User newUser(uname, pass, clientSock);
                users[uname] = newUser;
                saveUsers();
                sendMsg(clientSock, "Registration successful! Please LOGIN now.\n");
            }

        } else if (cmd == "QUIT") {
            close(clientSock);
            return;
        } else {
            sendMsg(clientSock, "Unknown command. Use LOGIN / REGISTER / QUIT\n");
        }
    }

    // Logged in menu
    sendMsg(clientSock, "\nWelcome, " + username + "!\n"
                        "Choose: SUBSCRIBE / UNSUBSCRIBE / VIEW / CHPASS / LOGOUT\n");

    while (true) {
        std::string option = recvMsg(clientSock);
        std::lock_guard<std::mutex> lock(userMutex);

        if (option == "SUBSCRIBE") {
            sendMsg(clientSock, "Enter location: ");
            std::string loc = recvMsg(clientSock);
            users[username].subscribeLocation(loc);
            saveUsers();
            sendMsg(clientSock, "Subscribed to " + loc + "\n");

        } else if (option == "UNSUBSCRIBE") {
            sendMsg(clientSock, "Enter location: ");
            std::string loc = recvMsg(clientSock);
            users[username].unsubscribeLocation(loc);
            saveUsers();
            sendMsg(clientSock, "Unsubscribed from " + loc + "\n");

        } else if (option == "VIEW") {
            auto locs = users[username].getLocations();
            std::string result = "Your Subscriptions:\n";
            for (auto& loc : locs) result += "- " + loc + "\n";
            sendMsg(clientSock, result);

        } else if (option == "CHPASS") {
            sendMsg(clientSock, "Enter new password: ");
            std::string newPass = recvMsg(clientSock);
            users[username].setPassword(newPass);
            saveUsers();
            sendMsg(clientSock, "Password changed.\n");

        } else if (option == "LOGOUT") {
            users[username].setSocketNo(-1);
            sendMsg(clientSock, "Logged out. Bye!\n");
            break;

        } else {
            sendMsg(clientSock, "Unknown option.\n");
        }
    }

    close(clientSock);
}

int main() {
    loadUsers();
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server started on port " << PORT << "\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t addrlen = sizeof(clientAddr);
        int clientSock = accept(server_fd, (struct sockaddr*)&clientAddr, &addrlen);

        std::thread(handleClient, clientSock).detach(); // spawn new thread
    }

    close(server_fd);
    return 0;
}
