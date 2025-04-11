//client.cpp
#include <iostream>
#include <fstream>
#include <string.h>

#include "User.hpp"

void create_account(){  //Function to create an account in the server

    std::string new_username, new_password, new_id, new_pass;

    std::cout << "Creating a new account..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "Enter a valid username: ";
    std::cin >> new_username;
    std::cout << "\nEnter a valid password: ";
    std::cin >> new_password;

    ofstream user_info("user.txt", std::ios::app); //Appends the new user details to the user.txt file
    if(user_info.is_open()){
        user_info << new_username << " " << new_password << std::endl;
        std::cout << "Account created successfully!" << std::endl;
    } 
    else{
        std::cout << "Error creating account. Please try again." << std::endl;
    }

}

void login(){   //Function to login the user to the server

    int count = 0;

    std::string username, password, id, pass;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    std::ifstream input("user.txt"); //Adds user details to user.txt file
    
    while(input >> id >> pass){
        if(id == username && pass == password){

            count = 1;
            break;

        }
    }
    input.close();

    if(count == 1){
        std::cout << "Successful Login! Welcome, " << username << "!" << std::endl;
    } 
    else{
        std::cout << "Login failed. Invalid user ID or password." << std::endl;
    }

}

void forgot_password(){



}

int main(){

    int choice;
    bool running = true; 

    while(running){

        std::cout << "Welcome to your local weather station!\n";
        std::cout << "Please choose an option:\n";
        std::cout << "\t1. Login" << std::endl;
        std::cout << "\t2. Register" << std::endl;
        std::cout << "\t3. Exit" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch(choice){

            case 1:
                std::cout << "Login" << std::endl;
                login();
                break;

            case 2:
                std::cout << "Register" << std::endl;
                create_account();
                break;
        
            case 3:
                std::cout << "Exiting..." << std::endl;
                running = false;
                break;
        
            default:
                std::cout << "Option not recognized. Please pick an option from the menu." << std::endl;
                break;
        };

        return 0;
    }
    std::cout << "Exiting the program..." << std::endl;
}