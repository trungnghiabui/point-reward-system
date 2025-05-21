#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "User.h"

class OTPManager;

class UserManager {
private:
    std::unordered_map<std::string, User> users;  // Map username -> User
    std::string dataFilePath;                     // Đường dẫn file lưu dữ liệu

    // Helper methods
    bool validateUsername(const std::string& username) const;
    bool validatePassword(const std::string& password) const;
    bool validateEmail(const std::string& email) const;
    bool validatePhoneNumber(const std::string& phoneNumber) const;
    
public:
    UserManager(const std::string& dataFilePath);
    ~UserManager();

    // Quản lý dữ liệu
    void loadData();
    void saveData() const;
    
    // Tạo tài khoản và xác thực
    bool registerUser(const std::string& username, const std::string& password,
                     const std::string& fullName, const std::string& email,
                     const std::string& phoneNumber, const std::string& address);

    // Quản lý người dùng
    bool userExists(const std::string& username) const;
    User* getUser(const std::string& username);
    const User* getUser(const std::string& username) const;

    bool authenticateUser(const std::string& username, const std::string& password);
    
};

#endif // USER_MANAGER_H