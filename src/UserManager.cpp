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

bool UserManager::userExists(const std::string& username) const {
    return users.find(username) != users.end();
}

User* UserManager::getUser(const std::string& username) {
    if (userExists(username)) {
        return &users[username];
    }
    return nullptr;
}

const User* UserManager::getUser(const std::string& username) const {
    if (userExists(username)) {
        return &users.at(username);
    }
    return nullptr;
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
    
    // Tạo người dùng mới
    User newUser(username, PasswordManager::hashPassword(password), fullName, email, phoneNumber, address);
    
    // Lưu vào danh sách
    users[username] = newUser;
    
    // Lưu dữ liệu
    saveData();
    
    std::cout << "Đăng ký thành công!" << std::endl;
    return true;
}

bool UserManager::createUserByAdmin(const std::string& adminUsername,
                                   const std::string& newUsername,
                                   const std::string& fullName,
                                   const std::string& email,
                                   const std::string& phoneNumber,
                                   const std::string& address) {
    // Kiểm tra người dùng admin
    if (!userExists(adminUsername)) {
        std::cout << "Người quản lý không tồn tại!" << std::endl;
        return false;
    }
    
    User* admin = getUser(adminUsername);
    if (!admin->getIsAdmin()) {
        std::cout << "Người dùng " << adminUsername << " không có quyền quản lý!" << std::endl;
        return false;
    }
    
    // Kiểm tra tên người dùng mới
    if (!validateUsername(newUsername)) {
        std::cout << "Tên người dùng không hợp lệ! Yêu cầu 4-20 ký tự, chỉ gồm chữ cái, số và gạch dưới." << std::endl;
        return false;
    }
    
    // Kiểm tra xem tên người dùng đã tồn tại chưa
    if (userExists(newUsername)) {
        std::cout << "Tên người dùng đã tồn tại!" << std::endl;
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
    
    // Tạo mật khẩu tạm thời
    std::string tempPassword = PasswordManager::generateRandomPassword(10);
    std::string passwordHash = PasswordManager::hashPassword(tempPassword);
    
    // Tạo người dùng mới
    User newUser(newUsername, passwordHash, fullName, email, phoneNumber, address);
    newUser.setPasswordTemporary(true);
    
    // Lưu vào danh sách
    users[newUsername] = newUser;
    
    // Lưu dữ liệu
    saveData();
    
    std::cout << "Tạo người dùng thành công!" << std::endl;
    std::cout << "Tên đăng nhập: " << newUsername << std::endl;
    std::cout << "Mật khẩu tạm thời: " << tempPassword << std::endl;
    std::cout << "Người dùng sẽ được yêu cầu thay đổi mật khẩu khi đăng nhập lần đầu." << std::endl;
    
    return true;
}

bool UserManager::authenticateUser(const std::string& username, const std::string& password) {
    // Kiểm tra người dùng tồn tại
    if (!userExists(username)) {
        std::cout << "Tên người dùng không tồn tại!" << std::endl;
        return false;
    }
    
    User* user = getUser(username);
    
    // Kiểm tra mật khẩu
    if (!PasswordManager::verifyPassword(password, user->getPasswordHash())) {
        std::cout << "Mật khẩu không đúng!" << std::endl;
        return false;
    }
    
    // Cập nhật thời gian đăng nhập cuối
    user->updateLastLogin();
    saveData();
    
    std::cout << "Đăng nhập thành công!" << std::endl;
    
    // Kiểm tra mật khẩu tạm thời
    if (user->isPasswordTemporary()) {
        std::cout << "Bạn đang sử dụng mật khẩu tạm thời. Vui lòng thay đổi mật khẩu ngay." << std::endl;
    }
    
    return true;
}

// Danh sách người dùng
std::vector<User> UserManager::getAllUsers() const {
    std::vector<User> userList;
    
    for (const auto& [username, user] : users) {
        userList.push_back(user);
    }
    
    return userList;
}

std::vector<User> UserManager::searchUsers(const std::string& searchTerm) const {
    std::vector<User> results;
    
    // Tìm kiếm không phân biệt chữ hoa/thường
    std::string lowerSearchTerm = searchTerm;
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);
    
    for (const auto& [username, user] : users) {
        // Chuyển các trường cần tìm kiếm thành chữ thường
        std::string lowerUsername = username;
        std::transform(lowerUsername.begin(), lowerUsername.end(), lowerUsername.begin(), ::tolower);
        
        std::string lowerFullName = user.getFullName();
        std::transform(lowerFullName.begin(), lowerFullName.end(), lowerFullName.begin(), ::tolower);
        
        std::string lowerEmail = user.getEmail();
        std::transform(lowerEmail.begin(), lowerEmail.end(), lowerEmail.begin(), ::tolower);
        
        // Tìm kiếm trong các trường
        if (lowerUsername.find(lowerSearchTerm) != std::string::npos ||
            lowerFullName.find(lowerSearchTerm) != std::string::npos ||
            lowerEmail.find(lowerSearchTerm) != std::string::npos ||
            user.getPhoneNumber().find(searchTerm) != std::string::npos) {
            results.push_back(user);
        }
    }
    
    return results;
}