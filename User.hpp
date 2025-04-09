#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
using namespace std;

class User {
private:
    string username;
    string password;
    int socketNo;
    vector<string> subscribedLocations;
    vector<string> receivedMessages;

public:
    User();
    User(const string& uname, const string& pwd, int sock);

    // Getters
    string getUsername() const;
    string getPassword() const;
    int getSocketNo() const;
    vector<string> getSubscribedLocations() const;
    vector<string> getLast10Messages() const;

    // Setters
    void setPassword(const string& newPassword);
    void setSocketNo(int sock);

    // Location methods
    void subscribeToLocation(const string& location);
    void unsubscribeFromLocation(const string& location);
    bool isSubscribedTo(const string& location) const;

    // Message methods
    void addMessage(const string& message);
};

#endif
