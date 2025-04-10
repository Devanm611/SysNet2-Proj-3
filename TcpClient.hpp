#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>
using namespace std;

class TcpClient {// TCP client class

private:
    int sockfd; // socket file descriptor

public:
    TcpClient();
    ~TcpClient();

    bool connectToServer(const string& ip, int port);// connects to the server
    bool sendData(const string& data); // sends data to the server
    string receiveData(); // receives data from the server
    int getSocket() const; // returns the socket file descriptor

};


#endif
