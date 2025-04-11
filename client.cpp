#include "TcpClient.hpp"
#include <iostream>
#include <string>

void showMainMenu() {
    std::cout << "\nMain Menu:\n";
    std::cout << "1. LOGIN\n";
    std::cout << "2. REGISTER\n";
    std::cout << "3. QUIT\n";
    std::cout << "Choose: ";
}

void showUserMenu() {
    std::cout << "\nUser Menu:\n";
    std::cout << "1. SUBSCRIBE\n";
    std::cout << "2. UNSUBSCRIBE\n";
    std::cout << "3. VIEW (your subscriptions)\n";
    std::cout << "4. CHPASS (change password)\n";
    std::cout << "5. LOGOUT\n";
    std::cout << "Choose: ";
}

int main() {
    TcpClient client("127.0.0.1", 12345);
    if (!client.connectToServer()) {
        std::cerr << "Connection failed.\n";
        return 1;
    }

    std::string response = client.receiveData();
    std::cout << response;

    bool running = true;
    bool loggedIn = false;

    while (running) {
        if (!loggedIn) {
            showMainMenu();
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1") {
                client.sendData("LOGIN");
                std::cout << client.receiveData(); // Username prompt
                std::string uname;
                std::getline(std::cin, uname);
                client.sendData(uname);

                std::cout << client.receiveData(); // Password prompt
                std::string pass;
                std::getline(std::cin, pass);
                client.sendData(pass);

                std::string loginResponse = client.receiveData();
                std::cout << loginResponse;
                if (loginResponse.find("successful") != std::string::npos) {
                    loggedIn = true;
                }

            } else if (choice == "2") {
                client.sendData("REGISTER");
                std::cout << client.receiveData(); // Username prompt
                std::string uname;
                std::getline(std::cin, uname);
                client.sendData(uname);

                std::cout << client.receiveData(); // Password prompt
                std::string pass;
                std::getline(std::cin, pass);
                client.sendData(pass);

                std::cout << client.receiveData(); // Success or error

            } else if (choice == "3") {
                client.sendData("QUIT");
                running = false;
            } else {
                std::cout << "Invalid choice.\n";
            }
        } else {
            std::cout << client.receiveData(); // Show logged-in menu
            showUserMenu();
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1") {
                client.sendData("SUBSCRIBE");
                std::cout << client.receiveData(); // Enter location
                std::string loc;
                std::getline(std::cin, loc);
                client.sendData(loc);
                std::cout << client.receiveData();

            } else if (choice == "2") {
                client.sendData("UNSUBSCRIBE");
                std::cout << client.receiveData();
                std::string loc;
                std::getline(std::cin, loc);
                client.sendData(loc);
                std::cout << client.receiveData();

            } else if (choice == "3") {
                client.sendData("VIEW");
                std::cout << client.receiveData();

            } else if (choice == "4") {
                client.sendData("CHPASS");
                std::cout << client.receiveData();
                std::string newpass;
                std::getline(std::cin, newpass);
                client.sendData(newpass);
                std::cout << client.receiveData();

            } else if (choice == "5") {
                client.sendData("LOGOUT");
                std::cout << client.receiveData();
                loggedIn = false;
            } else {
                std::cout << "Invalid option.\n";
            }
        }
    }

    client.closeConnection();
    return 0;
}
