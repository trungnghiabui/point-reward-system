#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include <string>
#include <unordered_map>

class UserManager;

class AuthenticationManager {
private:
    UserManager& userManager;
    
    const int MAX_LOGIN_ATTEMPTS = 5;
    std::unordered_map<std::string, int> loginAttempts; // Theo dõi số lần đăng nhập sai
    std::unordered_map<std::string, bool> loggedInUsers; // Theo dõi trạng thái đăng nhập
    
public:
    AuthenticationManager(UserManager& userManager);
    
    // Đăng nhập cơ bản
    bool login(const std::string& username, const std::string& password);
    
    // Kiểm tra trạng thái đăng nhập
    bool isLoggedIn(const std::string& username) const;

    // Xử lý khi đăng nhập sai nhiều lần
    void handleFailedLogin(const std::string& username);
};

#endif // AUTHENTICATION_MANAGER_H