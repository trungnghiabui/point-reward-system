#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include <string>
#include <unordered_map>

class UserManager;
class OTPManager;

class AuthenticationManager {
private:
    UserManager& userManager;
    OTPManager& otpManager;
    
    const int MAX_LOGIN_ATTEMPTS = 5;
    std::unordered_map<std::string, int> loginAttempts; // Theo dõi số lần đăng nhập sai
    std::unordered_map<std::string, bool> loggedInUsers; // Theo dõi trạng thái đăng nhập
    
public:
    AuthenticationManager(UserManager& userManager, OTPManager& otpManager);
    
    // Đăng nhập cơ bản
    bool login(const std::string& username, const std::string& password);
    
    // Đăng nhập với OTP (cho hoạt động nhạy cảm)
    bool loginWithOTP(const std::string& username, const std::string& password, const std::string& otp);
    
    // Khởi tạo quá trình xác thực 2 lớp
    bool initiateOTPAuthentication(const std::string& username);
    
    // Đăng xuất
    void logout(const std::string& username);
    
    // Kiểm tra trạng thái đăng nhập
    bool isLoggedIn(const std::string& username) const;
    
    // Xử lý khi đăng nhập sai nhiều lần
    void handleFailedLogin(const std::string& username);
    
    // Đặt lại số lần đăng nhập sai
    void resetLoginAttempts(const std::string& username);
};

#endif // AUTHENTICATION_MANAGER_H