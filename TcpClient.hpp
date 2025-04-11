#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>

class TcpClient {
private:
    int sockfd;
    std::string serverIp;
    int serverPort;

public:
    TcpClient();
    ~TcpClient();

    bool connectToServer(const std::string& ip, int port);
    bool sendData(const std::string& data);
    std::string receiveData();
    int getSocket() const;
};

#endif
