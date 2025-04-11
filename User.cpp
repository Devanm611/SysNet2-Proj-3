//User.cpp
#include <algorithm>
#include "User.hpp"

// Default constructor
User::User() : username(""), password(""), socketNo(-1) {}

User::User(const std::string& uname, const std::string& pwd, int sock)
    : username(uname), password(pwd), socketNo(sock) {}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

int User::getSocketNo() const {
    return socketNo;
}

std::vector<std::string> User::getSubscribedLocations() const {
    return subscribedLocations;
}

std::vector<std::string> User::getLast10Messages() const {
    if (receivedMessages.size() <= 10)
        return receivedMessages;
    return std::vector<std::string>(receivedMessages.end() - 10, receivedMessages.end());
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

void User::setSocketNo(int sock) {
    socketNo = sock;
}

void User::subscribeToLocation(const std::string& location) {
    if (!isSubscribedTo(location)) {
        subscribedLocations.push_back(location);
    }
}

void User::unsubscribeFromLocation(const std::string& location) {
    subscribedLocations.erase(
        std::remove(subscribedLocations.begin(), subscribedLocations.end(), location),
        subscribedLocations.end()
    );
}

bool User::isSubscribedTo(const std::string& location) const {
    return std::find(subscribedLocations.begin(), subscribedLocations.end(), location) != subscribedLocations.end();
}

void User::addMessage(const std::string& message) {
    receivedMessages.push_back(message);
}
