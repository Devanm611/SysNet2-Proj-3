#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connectToServer(const std::string& ip, int port);
    bool sendData(const std::string& data);
    std::string receiveData();
    int getSocket() const;

private:
    int sockfd;
};

#endif
