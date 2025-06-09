#include "../include/AuthenticationManager.h"
#include "../include/UserManager.h"
#include "../include/OTPManager.h"
#include <iostream>

AuthenticationManager::AuthenticationManager(UserManager& userManager, OTPManager& otpManager)
    : userManager(userManager), otpManager(otpManager) {
}

// Đăng nhập cơ bản
bool AuthenticationManager::login(const std::string& username, const std::string& password) {
    // Kiểm tra xem người dùng đã đăng nhập chưa
    if (isLoggedIn(username)) {
        std::cout << "Người dùng đã đăng nhập!" << std::endl;
        return true;
    }
    
    // Kiểm tra số lần đăng nhập sai
    if (loginAttempts.find(username) != loginAttempts.end() && loginAttempts[username] >= MAX_LOGIN_ATTEMPTS) {
        std::cout << "Tài khoản đã bị khóa do đăng nhập sai nhiều lần!" << std::endl;
        std::cout << "Vui lòng liên hệ quản trị viên để mở khóa." << std::endl;
        return false;
    }
    
    // Thực hiện xác thực
    bool authenticated = userManager.authenticateUser(username, password);
    
    if (authenticated) {
        // Đăng nhập thành công
        loggedInUsers[username] = true;
        resetLoginAttempts(username);
        
        // Kiểm tra xem người dùng có sử dụng mật khẩu tạm thời không
        if (userManager.checkAndPromptPasswordChange(username)) {
            std::cout << "Bạn đang sử dụng mật khẩu tạm thời. Vui lòng thay đổi mật khẩu ngay." << std::endl;
        }
    } else {
        // Đăng nhập thất bại
        handleFailedLogin(username);
    }
    
    return authenticated;
}

// Đăng nhập với OTP (cho hoạt động nhạy cảm)
bool AuthenticationManager::loginWithOTP(const std::string& username, const std::string& password, const std::string& otp) {
    // Kiểm tra đăng nhập cơ bản trước
    if (!userManager.authenticateUser(username, password)) {
        return false;
    }
    
    // Xác thực OTP
    if (!otpManager.verifyOTP(username, otp)) {
        std::cout << "Mã OTP không đúng hoặc đã hết hạn!" << std::endl;
        return false;
    }
    
    // Đăng nhập thành công
    loggedInUsers[username] = true;
    resetLoginAttempts(username);
    
    std::cout << "Đăng nhập với xác thực 2 lớp thành công!" << std::endl;
    return true;
}

// Khởi tạo quá trình xác thực 2 lớp
bool AuthenticationManager::initiateOTPAuthentication(const std::string& username) {
    if (!userManager.userExists(username)) {
        std::cout << "Người dùng không tồn tại!" << std::endl;
        return false;
    }
    
    // Tạo và gửi OTP
    std::string otp = otpManager.generateOTP(username);
    otpManager.sendOTP(username, otp);
    
    std::cout << "Đã gửi mã OTP đến người dùng " << username << std::endl;
    return true;
}

// Đăng xuất
void AuthenticationManager::logout(const std::string& username) {
    if (loggedInUsers.find(username) != loggedInUsers.end()) {
        loggedInUsers.erase(username);
        std::cout << "Đã đăng xuất khỏi tài khoản " << username << std::endl;
    }
}

// Kiểm tra trạng thái đăng nhập
bool AuthenticationManager::isLoggedIn(const std::string& username) const {
    return loggedInUsers.find(username) != loggedInUsers.end() && loggedInUsers.at(username);
}

// Xử lý khi đăng nhập sai nhiều lần
void AuthenticationManager::handleFailedLogin(const std::string& username) {
    // Tăng số lần đăng nhập sai
    if (loginAttempts.find(username) == loginAttempts.end()) {
        loginAttempts[username] = 1;
    } else {
        loginAttempts[username]++;
    }
    
    int remainingAttempts = MAX_LOGIN_ATTEMPTS - loginAttempts[username];
    
    if (remainingAttempts <= 0) {
        std::cout << "Tài khoản đã bị khóa do đăng nhập sai nhiều lần!" << std::endl;
        std::cout << "Vui lòng liên hệ quản trị viên để mở khóa." << std::endl;
    } else {
        std::cout << "Đăng nhập thất bại! Còn " << remainingAttempts << " lần thử." << std::endl;
    }
}

// Đặt lại số lần đăng nhập sai
void AuthenticationManager::resetLoginAttempts(const std::string& username) {
    if (loginAttempts.find(username) != loginAttempts.end()) {
        loginAttempts.erase(username);
    }
}