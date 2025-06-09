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
    OTPManager& otpManager;                      // Tham chiếu đến OTPManager
    
    // Helper methods
    bool validateUsername(const std::string& username) const;
    bool validatePassword(const std::string& password) const;
    bool validateEmail(const std::string& email) const;
    bool validatePhoneNumber(const std::string& phoneNumber) const;
    
public:
    UserManager(const std::string& dataFilePath, OTPManager& otpManager);
    ~UserManager();
    
    // Quản lý dữ liệu
    void loadData();
    void saveData() const;
    
    // Tạo tài khoản và xác thực
    bool registerUser(const std::string& username, const std::string& password,
                     const std::string& fullName, const std::string& email,
                     const std::string& phoneNumber, const std::string& address);
    
    // Tạo tài khoản bởi admin với mật khẩu tự động
    bool createUserByAdmin(const std::string& adminUsername,
                          const std::string& newUsername,
                          const std::string& fullName,
                          const std::string& email,
                          const std::string& phoneNumber,
                          const std::string& address);
    
    bool authenticateUser(const std::string& username, const std::string& password);
    
    // Quản lý người dùng
    bool userExists(const std::string& username) const;
    User* getUser(const std::string& username);
    const User* getUser(const std::string& username) const;
    bool deleteUser(const std::string& username);
    
    // Thay đổi mật khẩu
    bool changeUserPassword(const std::string& username, 
                           const std::string& oldPassword, 
                           const std::string& newPassword);
    
    // Kiểm tra và yêu cầu thay đổi mật khẩu tạm thời nếu cần
    bool checkAndPromptPasswordChange(const std::string& username);
    
    // Thiết lập lại mật khẩu (dành cho admin)
    bool resetUserPassword(const std::string& adminUsername, 
                          const std::string& targetUsername);
    
    // Cập nhật thông tin người dùng
    bool requestUserInfoUpdate(const std::string& username, 
                              const std::string& newFullName, 
                              const std::string& newEmail,
                              const std::string& newPhoneNumber, 
                              const std::string& newAddress);
    
    // Xác nhận thay đổi với OTP
    bool confirmUserInfoUpdate(const std::string& username, const std::string& otp);
    
    // Admin điều chỉnh thông tin của người dùng khác
    bool requestUserInfoUpdateByAdmin(const std::string& adminUsername,
                                    const std::string& targetUsername,
                                    const std::string& newFullName,
                                    const std::string& newEmail,
                                    const std::string& newPhoneNumber,
                                    const std::string& newAddress);
    
    // Danh sách người dùng (cho admin)
    std::vector<User> getAllUsers() const;
    std::vector<User> searchUsers(const std::string& searchTerm) const;
};

#endif // USER_MANAGER_H