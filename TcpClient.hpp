#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>

class TcpClient {
private:
    std::string serverIP;  // declared in correct order
    int serverPort;
    int sockfd;

public:
    TcpClient(const std::string& ip, int port);
    ~TcpClient();

    bool connectToServer();
    bool sendData(const std::string& data);
    std::string receiveData();
    void closeConnection();

    int getSocket() const;
};

#endif
