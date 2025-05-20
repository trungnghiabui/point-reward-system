#include "../include/PasswordManager.h"
#include <functional>

// Kiểm tra độ mạnh của mật khẩu
bool PasswordManager::isStrongPassword(const std::string& password) {
    // Yêu cầu ít nhất 8 ký tự
    if (password.length() < 8) {
        return false;
    }
    
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    
    for (char c : password) {
        if (islower(c)) hasLower = true;
        else if (isupper(c)) hasUpper = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }
    
    // Yêu cầu ít nhất 3 trong 4 loại ký tự
    int categories = hasLower + hasUpper + hasDigit + hasSpecial;
    return categories >= 3;
}