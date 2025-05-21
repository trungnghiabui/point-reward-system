#include "../include/User.h"
#include <sstream>
#include <iostream>

// Constructors
User::User() : 
    isAdmin(false), 
    createdAt(time(nullptr)), 
    lastLogin(0), 
    passwordIsTemporary(false) {
}

User::User(const std::string& username, const std::string& passwordHash) : 
    username(username), 
    passwordHash(passwordHash), 
    isAdmin(false), 
    createdAt(time(nullptr)), 
    lastLogin(0), 
    passwordIsTemporary(false) {
}

User::User(const std::string& username, const std::string& passwordHash, 
           const std::string& fullName, const std::string& email, 
           const std::string& phoneNumber, const std::string& address) : 
    username(username), 
    passwordHash(passwordHash), 
    fullName(fullName), 
    email(email), 
    phoneNumber(phoneNumber), 
    address(address), 
    isAdmin(false), 
    createdAt(time(nullptr)), 
    lastLogin(0), 
    passwordIsTemporary(false) {
}

// Getters & Setters
std::string User::getUsername() const {
    return username;
}

std::string User::getPasswordHash() const {
    return passwordHash;
}

std::string User::getFullName() const {
    return fullName;
}

void User::setFullName(const std::string& fullName) {
    this->fullName = fullName;
}

std::string User::getEmail() const {
    return email;
}

void User::setEmail(const std::string& email) {
    this->email = email;
}

std::string User::getPhoneNumber() const {
    return phoneNumber;
}

void User::setPhoneNumber(const std::string& phoneNumber) {
    this->phoneNumber = phoneNumber;
}

std::string User::getAddress() const {
    return address;
}

void User::setAddress(const std::string& address) {
    this->address = address;
}

bool User::getIsAdmin() const {
    return isAdmin;
}

void User::setIsAdmin(bool isAdmin) {
    this->isAdmin = isAdmin;
}

time_t User::getCreatedAt() const {
    return createdAt;
}

time_t User::getLastLogin() const {
    return lastLogin;
}

void User::updateLastLogin() {
    lastLogin = time(nullptr);
}

bool User::isPasswordTemporary() const {
    return passwordIsTemporary;
}

void User::setPasswordTemporary(bool isTemporary) {
    passwordIsTemporary = isTemporary;
}

std::string User::getWalletId() const {
    return walletId;
}

void User::setWalletId(const std::string& walletId) {
    this->walletId = walletId;
}

bool User::hasWallet() const {
    return !walletId.empty();
}

// Serualization
std::string User::serialize() const {
    std::stringstream ss;
    ss << username << "\n";
    ss << passwordHash << "\n";
    ss << fullName << "\n";
    ss << email << "\n";
    ss << phoneNumber << "\n";
    ss << address << "\n";
    ss << (isAdmin ? "1" : "0") << "\n";
    ss << createdAt << "\n";
    ss << lastLogin << "\n";
    ss << (passwordIsTemporary ? "1" : "0") << "\n";
    
    return ss.str();
}

User User::deserialize(const std::string& data) {
    std::istringstream iss(data);
    User user;
    
    std::getline(iss, user.username);
    std::getline(iss, user.passwordHash);
    std::getline(iss, user.fullName);
    std::getline(iss, user.email);
    std::getline(iss, user.phoneNumber);
    std::getline(iss, user.address);
    
    std::string temp;
    std::getline(iss, temp);
    user.isAdmin = (temp == "1");
        
    std::getline(iss, temp);
    user.createdAt = std::stol(temp);
    
    std::getline(iss, temp);
    user.lastLogin = std::stol(temp);
    
    std::getline(iss, temp);
    user.passwordIsTemporary = (temp == "1");
    
    return user;
}