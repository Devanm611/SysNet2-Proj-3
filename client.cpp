//client.cpp
#include <iostream>
#include <string>
#include "TcpClient.hpp"

bool logged_in = false;
TcpClient client;
std::string current_user;

void create_account() {
    std::string new_username, new_password;

    std::cout << "Creating a new account..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "Enter a valid username: ";
    std::cin >> new_username;
    std::cout << "\nEnter a valid password: ";
    std::cin >> new_password;

    std::string request = "Register " + new_username + " " + new_password;
    client.sendData(request);
    std::string response = client.receiveData();
    std::cout << response << std::endl;
}

void login() {
    while(!logged_in){
        std::string password;
        std::cout << "Username: ";
        std::cin >> current_user;
        std::cout << "Password: ";
        std::cin >> password;

        std::string request = "Login " + current_user + " " + password;
        client.sendData(request);
        std::string response = client.receiveData();
        std::cout << response << std::endl;

        if(response.find("SUCCESS") != std::string::npos){
            std::cout << "Successful Login! Welcome, " << current_user << "!" << std::endl;
            std::cout << "___________________________________" << std::endl;
            logged_in = true;
        } 
        else{
            std::cout << "Login failed. Invalid user ID or password." << std::endl;
            std::cout << "Please try again or Register your details.\n" << std::endl;
            return;
        }
    }
}

void change_password() {
    std::string oldpass, newpass;

    std::cout << "Changing your password..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "Enter your current password: ";
    std::cin >> oldpass;
    std::cout << "Enter your new password: ";
    std::cin >> newpass;

    std::string request = "ChangePassword " + current_user + " " + oldpass + " " + newpass;
    client.sendData(request);
    std::string response = client.receiveData();
    std::cout << response << std::endl;

    if(response.find("SUCCESS") != std::string::npos){
        std::cout << "Password changed successfully!" << std::endl;
        logged_in = false;
        std::cout << "Please login again to continue." << std::endl;
    }
}

void subscribe_to_location(){
    std::cout << "Subscribing to a location..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "Available locations: \nPensacola, Destin, Fort Walton Beach, Crestview, Navarre." << std::endl;
    std::cout << "Enter the location you want to subscribe to: ";
    
    std::string location;
    std::cin >> location;

    if(location == "Pensacola" || location == "Destin" || location == "Fort Walton Beach" || location == "Crestview" || location == "Navarre"){
        std::string request = "Subscribe " + location;
        client.sendData(request);
        
        std::string response = client.receiveData();
        std::cout << response << std::endl;

        if(response.find("SUCCESS") != std::string::npos){
            std::cout << "Successfully subscribed to " << location << "!" << std::endl;
        } 
        else{
            std::cout << "Subscription failed. Please try again." << std::endl;
        }
    } 
    else{
        std::cout << "\nInvalid location. Please choose from the following: \nPensacola, Destin, Fort Walton Beach, Crestview, Navarre.\n" << std::endl;
        return;
    }

}

void unsubscribe_from_location(){
    std::cout << "Unsubscribing from a location..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::string location;
    std::cout << "Enter the location you want to unsubscribe from: ";
    std::cin >> location;

    if(location == "Pensacola" || location == "Destin" || location == "Fort Walton Beach" || location == "Crestview" || location == "Navarre"){
        std::string request = "Unsubscribe " + current_user + " " + location;
        client.sendData(request);
        std::string response = client.receiveData();
        std::cout << response << std::endl;

        if(response.find("SUCCESS") != std::string::npos){
            std::cout << "Successfully unsubscribed from " << location << "!" << std::endl;
        } 
        else{
            std::cout << "Location not recognized. Please enter a valid location subscription to unsubscribe." << std::endl;
        }
    } 
    else{
        std::cout << "\nInvalid location. Please choose from the following: \nPensacola, Destin, Fort Walton Beach, Crestview, Navarre." << std::endl;
    }
    std::cout << "Unsubscribed from " << location << std::endl;
    std::cout << "___________________________________" << std::endl;
}

void display_subscriptions() {
    std::cout << "Displaying subscriptions... [TODO]" << std::endl;
}

int main() {
    if (!client.connectToServer("127.0.0.1", 8080)) {
        std::cerr << "Unable to connect to server." << std::endl;
        return 1;
    }

    int choice;
    bool running = true;

    while (running) {
        std::cout << "Welcome to your local weather station!\n";
        std::cout << "Please choose an option:\n";
        std::cout << "\t1. Login" << std::endl;
        std::cout << "\t2. Register" << std::endl;
        std::cout << "\t3. Exit" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                std::cout << "--Login--" << std::endl;
                login();
                while (logged_in) {
                    std::cout << "Please choose an option:\n";
                    std::cout << "\t1. Subscribe to a location" << std::endl;
                    std::cout << "\t2. Unsubscribe from a location" << std::endl;
                    std::cout << "\t3. Change Password" << std::endl;
                    std::cout << "\t4. See my Subscriptions" << std::endl;
                    std::cout << "\t5. Exit" << std::endl;

                    int sub_choice;
                    std::cin >> sub_choice;
                    std::cout << std::endl;

                    switch (sub_choice) {
                        case 1:
                            subscribe_to_location();
                            break;
                        case 2:
                            unsubscribe_from_location();
                            break;
                        case 3:
                            change_password();
                            break;
                        case 4:
                            display_subscriptions();
                            break;
                        case 5:
                            std::cout << "Logging out...\n" << std::endl;
                            logged_in = false;
                            break;
                        default:
                            std::cout << "Invalid option. Please try again." << std::endl;
                            break;
                    }
                }
                break;

            case 2:
                std::cout << "--Register--" << std::endl;
                create_account();
                break;

            case 3:
                client.sendData("Exit");
                std::cout << "Exiting..." << std::endl;
                running = false;
                break;

            default:
                std::cout << "Option not recognized. Please pick an option from the menu." << std::endl;
                break;
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
