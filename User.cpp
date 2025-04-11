#include "User.hpp"
#include <algorithm>

// Default constructor
User::User() : username(""), password(""), socketNo(-1) {}

// Parameterized constructor
User::User(const std::string& uname, const std::string& pass, int sock)
    : username(uname), password(pass), socketNo(sock) {}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

int User::getSocketNo() const {
    return socketNo;
}

void User::setPassword(const std::string& newPass) {
    password = newPass;
}

void User::setSocketNo(int sock) {
    socketNo = sock;
}

void User::subscribeLocation(const std::string& loc) {
    if (std::find(locations.begin(), locations.end(), loc) == locations.end()) {
        locations.push_back(loc);
    }
}

void User::unsubscribeLocation(const std::string& loc) {
    locations.erase(std::remove(locations.begin(), locations.end(), loc), locations.end());
}

std::vector<std::string> User::getLocations() const {
    return locations;
}

void User::addMessage(const std::string& msg) {
    messages.push_back(msg);
    if (messages.size() > 10) {
        messages.erase(messages.begin()); // keep only last 10 messages
    }
}

std::vector<std::string> User::getMessages() const {
    return messages;
}
