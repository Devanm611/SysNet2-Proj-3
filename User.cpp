#include "User.hpp"
#include <algorithm>
using namespace std;

User::User() : username(""), password(""), socketNo(-1) {}

User::User(const string& uname, const string& pwd, int sock)
    : username(uname), password(pwd), socketNo(sock) {}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

int User::getSocketNo() const {
    return socketNo;
}

vector<string> User::getSubscribedLocations() const {
    return subscribedLocations;
}

vector<string> User::getLast10Messages() const {
    if (receivedMessages.size() <= 10)
        return receivedMessages;
    return vector<string>(receivedMessages.end() - 10, receivedMessages.end());
}

void User::setPassword(const string& newPassword) {
    password = newPassword;
}

void User::setSocketNo(int sock) {
    socketNo = sock;
}

void User::subscribeToLocation(const string& location) {
    if (!isSubscribedTo(location)) {
        subscribedLocations.push_back(location);
    }
}

void User::unsubscribeFromLocation(const string& location) {
    subscribedLocations.erase(remove(subscribedLocations.begin(), subscribedLocations.end(), location),
                              subscribedLocations.end());
}

bool User::isSubscribedTo(const string& location) const {
    return find(subscribedLocations.begin(), subscribedLocations.end(), location) != subscribedLocations.end();
}

void User::addMessage(const string& message) {
    receivedMessages.push_back(message);
}
