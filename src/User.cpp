#include "../include/User.h"
#include "../include/PasswordManager.h"
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

// Thay đổi mật khẩu
bool User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    // Kiểm tra mật khẩu cũ
    if (!PasswordManager::verifyPassword(oldPassword, passwordHash)) {
        std::cout << "Mật khẩu cũ không đúng!" << std::endl;
        return false;
    }
    
    // Kiểm tra mật khẩu mới có đủ mạnh không
    if (!PasswordManager::isStrongPassword(newPassword)) {
        std::cout << "Mật khẩu mới không đủ mạnh!" << std::endl;
        return false;
    }
    
    // Cập nhật mật khẩu
    passwordHash = PasswordManager::hashPassword(newPassword);
    passwordIsTemporary = false;
    
    std::cout << "Mật khẩu đã được thay đổi thành công!" << std::endl;
    return true;
}

// Quản lý thay đổi thông tin
void User::savePendingChanges(const std::string& newFullName, const std::string& newEmail,
                            const std::string& newPhoneNumber, const std::string& newAddress) {
    pendingChanges.fullName = newFullName;
    pendingChanges.email = newEmail;
    pendingChanges.phoneNumber = newPhoneNumber;
    pendingChanges.address = newAddress;
    pendingChanges.hasChanges = true;
}

void User::confirmPendingChanges() {
    if (pendingChanges.hasChanges) {
        fullName = pendingChanges.fullName;
        email = pendingChanges.email;
        phoneNumber = pendingChanges.phoneNumber;
        address = pendingChanges.address;
        
        // Xóa thông tin thay đổi tạm thời
        pendingChanges.hasChanges = false;
    }
}

void User::cancelPendingChanges() {
    pendingChanges.hasChanges = false;
}

bool User::hasPendingChanges() const {
    return pendingChanges.hasChanges;
}

User::PendingChanges User::getPendingChanges() const {
    return pendingChanges;
}

std::string User::getPendingChangesDescription() const {
    std::stringstream ss;
    ss << "Các thay đổi sẽ được áp dụng:\n";
    ss << "- Họ tên: " << (fullName != pendingChanges.fullName ? pendingChanges.fullName + " (thay đổi từ " + fullName + ")" : "Không thay đổi") << "\n";
    ss << "- Email: " << (email != pendingChanges.email ? pendingChanges.email + " (thay đổi từ " + email + ")" : "Không thay đổi") << "\n";
    ss << "- Số điện thoại: " << (phoneNumber != pendingChanges.phoneNumber ? pendingChanges.phoneNumber + " (thay đổi từ " + phoneNumber + ")" : "Không thay đổi") << "\n";
    ss << "- Địa chỉ: " << (address != pendingChanges.address ? pendingChanges.address + " (thay đổi từ " + address + ")" : "Không thay đổi");
    
    return ss.str();
}

// Lưu trữ và phục hồi
std::string User::serialize() const {
    std::stringstream ss;
    ss << username << "\n";
    ss << passwordHash << "\n";
    ss << fullName << "\n";
    ss << email << "\n";
    ss << phoneNumber << "\n";
    ss << address << "\n";
    ss << (isAdmin ? "1" : "0") << "\n";
    ss << walletId << "\n";
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
    
    std::getline(iss, user.walletId);
    
    std::getline(iss, temp);
    user.createdAt = std::stol(temp);
    
    std::getline(iss, temp);
    user.lastLogin = std::stol(temp);
    
    std::getline(iss, temp);
    user.passwordIsTemporary = (temp == "1");
    
    return user;
}
