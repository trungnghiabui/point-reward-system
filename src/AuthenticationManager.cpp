#include "../include/AuthenticationManager.h"
#include "../include/UserManager.h"
#include <iostream>

AuthenticationManager::AuthenticationManager(UserManager& userManager)
    : userManager(userManager) {
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
        loggedInUsers[username] = true;
    } else {
        // Đăng nhập thất bại
        handleFailedLogin(username);
    }
    
    return authenticated;
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