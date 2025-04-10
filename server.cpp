#include <iostream>
#include <fstream>
#include <string.h>

#include "User.cpp"

void create_account(){  //Function to create an account in the server

    std::string new_userId, new_password, new_id, new_pass;

    std::cout << "Creating a new account..." << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "Enter a valid username: ";
    std::cin >> new_userId;
    std::cout << "\nEnter a valid password: ";
    std::cin >> new_password;

    ofstream user_info("user.txt", std::ios::app); //Appends the new user details to the user.txt file
    if(user_info.is_open()){
        user_info << new_userId << " " << new_password << std::endl;
        std::cout << "Account created successfully!" << std::endl;
        main_menu();
    } 
    else{
        std::cout << "Error creating account. Please try again." << std::endl;
        main_menu();
    }

}

void login(){   //Function to login the user to the server

    int count;

    std::string user_ID, password, id, pass;
    std::cout << "Username: ";
    std::cin >> user_ID;
    std::cout << "Password: ";
    std::cin >> password;

    std::ifstream input("user.txt"); //Adds user details to user.txt file
    
    while(input >> id >> pass){
        if(id == userID && pass == password){

            count = 1;
            break;

        }
    }
    input.close();

    if(count == 1){
        std::cout << "Successful Login! Welcome, " << user_ID << "!" << std::endl;
        main_menu();
    } 
    else{
        std::cout << "Login failed. Invalid user ID or password." << std::endl;
        main_menu();
    }

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
            create_account();
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

