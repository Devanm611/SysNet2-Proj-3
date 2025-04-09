#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>
using namespace std;

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connectToServer(const string& ip, int port);
    bool sendData(const string& data);
    string receiveData();
    int getSocket() const;

private:
    int sockfd;
};

#endif
