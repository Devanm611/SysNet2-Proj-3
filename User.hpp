#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string password;
    int socketNo;
    std::vector<std::string> subscribedLocations;
    std::vector<std::string> receivedMessages;

public:
    User();
    User(const std::string& uname, const std::string& pwd, int sock);

    // Getters
    std::string getUsername() const;
    std::string getPassword() const;
    int getSocketNo() const;
    std::vector<std::string> getSubscribedLocations() const;
    std::vector<std::string> getLast10Messages() const;

    // Setters
    void setPassword(const std::string& newPassword);
    void setSocketNo(int sock);

    // Location methods
    void subscribeToLocation(const std::string& location);
    void unsubscribeFromLocation(const std::string& location);
    bool isSubscribedTo(const std::string& location) const;

    // Message methods
    void addMessage(const std::string& message);
};

#endif
