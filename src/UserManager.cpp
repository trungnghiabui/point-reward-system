#include "../include/UserManager.h"
#include "../include/PasswordManager.h"
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


bool UserManager::validatePassword(const std::string& password) const {
    return PasswordManager::isStrongPassword(password);
}

bool UserManager::validateEmail(const std::string& email) const {
    // Kiểm tra định dạng email cơ bản
    std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
}

bool UserManager::validatePhoneNumber(const std::string& phoneNumber) const {
    // Kiểm tra định dạng số điện thoại cơ bản (8-15 chữ số)
    std::regex pattern("^[0-9]{8,15}$");
    return std::regex_match(phoneNumber, pattern);
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

    // Kiểm tra xem tên người dùng đã tồn tại chưa
    if (userExists(username)) {
        std::cout << "Tên người dùng đã tồn tại!" << std::endl;
        return false;
    }

    // Kiểm tra mật khẩu
    if (!validatePassword(password)) {
        std::cout << "Mật khẩu không đủ mạnh! Yêu cầu ít nhất 8 ký tự và 3 loại ký tự khác nhau." << std::endl;
        return false;
    }

    // Kiểm tra email
    if (!validateEmail(email)) {
        std::cout << "Địa chỉ email không hợp lệ!" << std::endl;
        return false;
    }

    // Kiểm tra số điện thoại
    if (!validatePhoneNumber(phoneNumber)) {
        std::cout << "Số điện thoại không hợp lệ!" << std::endl;
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

bool UserManager::userExists(const std::string& username) const {
    return users.find(username) != users.end();
}