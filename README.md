# Project 3 - TCP Weather alert system - PART 1
## COP4635: System & Network 2
## Team Members: Devan Rivera & Ashley Villegas

## Project Description
This project implements a multi-threaded server that can handle multiple clients. The clients are able to login/logout, register/create an account, change password, subscribe/unsubscribe to a location, and see current subscriptions. 


**Required Files**
The following files are included in our directory:

- users.txt
- Makefile
- client.cpp
- server.cpp
- TcpClient.cpp
- TcpClient.hpp


## Running the Program (Using WSL: Ubuntu)

Step 1: Open 3 seperate WSL Ubuntu terminals 

Step 2: In any of the terminals run the make command. 

Step 3: In seperate terminals of your choosing run the commands:

- ./server (Establishes server)
- ./client (Establishes client 1)
- ./client (Establishes client 2)

Step 4: Follow the prompts in the client terminals to utilize the program. The server will appropriately respond to the clients.

Step 5: To exit the program, logout of your clients account (option 5), and exit the program from the login page (option 3).


