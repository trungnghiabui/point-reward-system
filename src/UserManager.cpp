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

// Tạo tài khoản và xác thực
bool UserManager::registerUser(const std::string& username, const std::string& password,
                              const std::string& fullName, const std::string& email,
                              const std::string& phoneNumber, const std::string& address) {
    
    // TODO : Kiểm tra tính hợp lệ của input
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