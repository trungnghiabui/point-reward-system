#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <ctime>

// Forward declaration
class Transaction;

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
    
    // Thông tin về ví điểm
    std::string walletId;        // ID của ví người dùng
    
    // Thông tin khác
    time_t createdAt;            // Thời gian tạo tài khoản
    time_t lastLogin;            // Thời gian đăng nhập gần nhất
    bool passwordIsTemporary;    // Đánh dấu mật khẩu tạm thời cần thay đổi
    
    // Thông tin thay đổi tạm thời (chờ xác nhận OTP)
    struct PendingChanges {
        std::string fullName;
        std::string email;
        std::string phoneNumber;
        std::string address;
        bool hasChanges;
        
        PendingChanges() : hasChanges(false) {}
    };
    
    PendingChanges pendingChanges;

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
    
    // Liên quan đến ví
    std::string getWalletId() const;
    void setWalletId(const std::string& walletId);
    bool hasWallet() const;
    
    // Thay đổi mật khẩu
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    
    // Quản lý thay đổi thông tin
    void savePendingChanges(const std::string& newFullName, const std::string& newEmail,
                            const std::string& newPhoneNumber, const std::string& newAddress);
    
    void confirmPendingChanges();
    void cancelPendingChanges();
    bool hasPendingChanges() const;
    PendingChanges getPendingChanges() const;
    std::string getPendingChangesDescription() const;
    
    // Lưu trữ và phục hồi
    std::string serialize() const;
    static User deserialize(const std::string& data);
};

#endif // USER_H