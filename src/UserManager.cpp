#include "../include/UserManager.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>

UserManager::UserManager(const std::string& dataFilePath) 
    : dataFilePath(dataFilePath) {
    loadData();
}

UserManager::~UserManager() {
    saveData();
}

// Quản lý dữ liệu
void UserManager::loadData() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "Không thể mở tệp dữ liệu người dùng. Tạo mới dữ liệu." << std::endl;
        return;
    }
    
    users.clear();
    
    std::string line;
    std::string userData;
    
    while (std::getline(file, line)) {
        if (line == "#USER") {
            userData.clear();
            
            while (std::getline(file, line) && line != "#END") {
                userData += line + "\n";
            }
            
            if (!userData.empty()) {
                User user = User::deserialize(userData);
                users[user.getUsername()] = user;
            }
        }
    }
    
    file.close();

}

void UserManager::saveData() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cerr << "Không thể mở tệp dữ liệu người dùng để lưu!" << std::endl;
        return;
    }
    
    for (const auto& [username, user] : users) {
        file << "#USER" << std::endl;
        file << user.serialize();
        file << "#END" << std::endl;
    }
    
    file.close();
}

// User validation
bool UserManager::validateUsername(const std::string& username) const {
    // Kiểm tra độ dài
    if (username.length() < 4 || username.length() > 20) {
        return false;
    }
    
    // Chỉ cho phép chữ cái, số và dấu gạch dưới
    std::regex pattern("^[a-zA-Z0-9_]+$");
    return std::regex_match(username, pattern);
}

// Tạo tài khoản và xác thực
bool UserManager::registerUser(const std::string& username, const std::string& password,
                              const std::string& fullName, const std::string& email,
                              const std::string& phoneNumber, const std::string& address) {
    
    // Kiểm tra tên người dùng
    if (!validateUsername(username)) {
        std::cout << "Tên người dùng không hợp lệ! Yêu cầu 4-20 ký tự, chỉ gồm chữ cái, số và gạch dưới." << std::endl;
        return false;
    }
    
    // TODO : Hashpassword
    // Tạo người dùng mới
    User newUser(username, password, fullName, email, phoneNumber, address);
    
    // Lưu vào danh sách
    users[username] = newUser;
    
    // Lưu dữ liệu
    saveData();
    
    std::cout << "Đăng ký thành công!" << std::endl;
    return true;
}