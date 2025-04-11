#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string password;
    int socketNo;
    std::vector<std::string> locations;
    std::vector<std::string> messages;

public:
    // Default constructor (required for std::map access)
    User();

    // Constructor with parameters
    User(const std::string& uname, const std::string& pass, int sock);

    // Setters and Getters
    std::string getUsername() const;
    std::string getPassword() const;
    int getSocketNo() const;

    void setPassword(const std::string& newPass);
    void setSocketNo(int sock);

    // Location management
    void subscribeLocation(const std::string& loc);
    void unsubscribeLocation(const std::string& loc);
    std::vector<std::string> getLocations() const;

    // Messages
    void addMessage(const std::string& msg);
    std::vector<std::string> getMessages() const;
};

#endif
