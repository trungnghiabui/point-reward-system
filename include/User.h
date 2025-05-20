#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <ctime>


class User {
private:
    // Thông tin xác thực
    std::string username;        // Tên đăng nhập, duy nhất trong hệ thống
    std::string passwordHash;    // Mật khẩu đã được mã hóa
    
    // Thông tin cá nhân
    std::string fullName;        // Họ và tên đầy đủ
    std::string email;           // Địa chỉ email
    std::string phoneNumber;     // Số điện thoại
    std::string address;         // Địa chỉ
    
    // Thông tin về vai trò
    bool isAdmin;                // True nếu là admin, false nếu là người dùng thông thường
    
    // Thông tin khác
    time_t createdAt;            // Thời gian tạo tài khoản
    time_t lastLogin;            // Thời gian đăng nhập gần nhất
    bool passwordIsTemporary;    // True nếu mật khẩu là tạm thời, false nếu mật khẩu đã được thay đổi

public:
    // Constructors
    User();
    User(const std::string& username, const std::string& passwordHash);

    User(const std::string& username, const std::string& passwordHash, 
        const std::string& fullName, const std::string& email, 
        const std::string& phoneNumber, const std::string& address);
    
    // Getters & Setters
    std::string getUsername() const;
    std::string getPasswordHash() const;
    std::string getFullName() const;
    void setFullName(const std::string& fullName);
    std::string getEmail() const;
    void setEmail(const std::string& email);
    std::string getPhoneNumber() const;
    void setPhoneNumber(const std::string& phoneNumber);
    std::string getAddress() const;
    void setAddress(const std::string& address);
    bool getIsAdmin() const;
    void setIsAdmin(bool isAdmin);
    time_t getCreatedAt() const;
    time_t getLastLogin() const;
    void updateLastLogin();
    bool isPasswordTemporary() const;
    void setPasswordTemporary(bool isTemporary);

    // Serialization
    std::string serialize() const;
    static User deserialize(const std::string& data);
};

#endif // USER_H