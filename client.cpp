#include <iostream>
#include <fstream>
#include <string.h>

#include "User.hpp"

bool logged_in = false; //Global variable to check if user is logged in

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
    while(logged_in == false){
        int count = 0;

        std::string username, password, id, pass;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        std::ifstream user_info("user.txt"); //Adds user details to user.txt file
    
        while(user_info >> id >> pass){
            if(id == username && pass == password){
                count = 1;
                break;
            }
            else{
                break;
            }
        }
        user_info.close();

        if(count == 1){
            std::cout << "Successful Login! Welcome, " << username << "!" << std::endl;
            std::cout << "___________________________________" << std::endl;
            logged_in = true; //Set logged_in to true to exit the loop
        } 
        else{
            std::cout << "Login failed. Invalid user ID or password." << std::endl;
        }

    }   
}
    

void change_password(){

    std::string username, password, change_password, id, pass;
    bool account_found = false;

    std::cout << "Enter your original username: ";
    std::cin >> username;
    std::cout << "\nEnter your original password: ";
    std::cin >> password;

    std::ifstream user_info("user.txt"); 
    std::ofstream temp_file("updates.txt"); //Temporary file for updated data

    while(user_info >> id >> pass){
        if(id == username && pass == password){
            account_found = true;
            std::cout << "\nEnter your new password: ";
            std::cin >> change_password;
            temp_file << id << " " << change_password << std::endl; //Write updated password to temporary file
            break;
        }
        else{
            std::cout << "No changes made." << std::endl;
            temp_file << id << " " << pass << std::endl; //If no changes were found update nothing
        }
    }

    user_info.close();
    temp_file.close();

    if(account_found = true){
        std::remove("user.txt"); //Delete the original file
        std::rename("updates.txt", "user.txt"); //Rename temp file to original file
        std::cout << "Password Successfully Updated!" << std::endl;
        logged_in = false; //Set logged_in to false to exit the loop
    } 
    else{
        std::remove("updates.txt"); //Remove temp file if user not found
        std::cout << "Invalid username or password. Password change failed." << std::endl;
    }

    std::cout << "Please login again to continue." << std::endl;
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
            int sub_choice;
            
            case 1:
                std::cout << "--Login--" << std::endl;
                login();
                while(logged_in == true){
                    std::cout << "Please choose an option:\n";
                    std::cout << "\t1. Subscribe to a location" << std::endl;
                    std::cout << "\t2. Unsubscribe from a location" << std::endl;
                    std::cout << "\t3. Change Password" << std::endl;
                    std::cout << "\t4. See my Subcriptions" << std::endl;
                    std::cout << "\t5. Exit" << std::endl;

                    std::cin >> sub_choice;
                    std::cout << std::endl;

                    switch(sub_choice){
                        case 1:
                            std::cout << "Subscribing to a location..." << std::endl;
                            std::cout << "___________________________________" << std::endl;
                            // Add subscription logic here
                            break;
                    
                        case 2:
                            std::cout << "Unsubscribing from a location..." << std::endl;
                            std::cout << "___________________________________" << std::endl;
                            // Add unsubscription logic here
                            break;

                        case 3:
                            std::cout << "Changing password..." << std::endl;
                            std::cout << "___________________________________" << std::endl;
                            change_password();
                            break;
                    
                        case 4:
                            std::cout << "Displaying subscriptions..." << std::endl;
                            std::cout << "___________________________________" << std::endl;
                            // Add logic to display subscriptions here
                            break;

                        case 5:
                            std::cout << "Exiting..." << std::endl;
                            logged_in = false; //Set logged_in to false to exit the loop
                            break;

                        default:
                            std::cout << "Option not recognized. Please pick an option from the menu." << std::endl;
                            break;
                    }
                }
                break;

            case 2:
                std::cout << "--Register--" << std::endl;
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