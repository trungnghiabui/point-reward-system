#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <string>

class PasswordManager {
public:
    // Kiểm tra độ mạnh của mật khẩu
    static bool isStrongPassword(const std::string& password);

    // Xác thực mật khẩu
    static bool verifyPassword(const std::string& inputPassword, const std::string& storedHash);

    // Mã hóa mật khẩu
    static std::string hashPassword(const std::string& password);
};

#endif // PASSWORD_MANAGER_H