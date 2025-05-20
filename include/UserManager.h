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
    
};

#endif // USER_MANAGER_H