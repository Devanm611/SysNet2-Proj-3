#include <iostream>
#include <fstream>
#include <string.h>

#include "User.cpp"

void login(){

    int count;

    std::string userID, password, id, pass;
    std::cout << "Enter your user ID: ";
    std::cin >> userID;
    std::cout << "Enter your password: ";
    std::cin >> password;

    std::ifstream input("user.txt"); //Adds user
    while (input >> id >> pass){
        if(id == userID && pass == password){

            count = 1;
            break;

        }
    }
    input.close();

    if (count == 1) {
        std::cout << "Login successful! Welcome, " << userID << "!" << std::endl;
    } else {
        std::cout << "Login failed. Invalid user ID or password." << std::endl;
    }

}

void registration(){



}

void forgot_password(){



}

int main(){

    int choice;
    
    std::cout << "Welcome to your local weather station!\n";
    std::cout << "Please choose an option:\n";
    std::cout << "\t1. Login" << std::endl;
    std::cout << "\t2. Register" << std::endl;
    std::cout << "\tType 'exit' to Quit" << std::endl;

    std::cin >> choice;
    std::cout << std::endl;

    switch(choice){

        case 1:
            std::cout << "Login" << std::endl;
            login();
            break;

        case 2:
            std::cout << "Register" << std::endl;
            registration();
            break;
        
        case 3:
            std::cout << "Forgot Password" << std::endl;
            forgot_password();
            break;

        case 4:
            std::cout << "Exit" << std::endl;
            break;
        
        default:
            std::cout << "Option not recognized. Please pick an option from the menu." << std::endl;
            break;
    }

}

