#include "../include/PasswordManager.h"
#include "sha256.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <functional>
#include <random>
#include <ctime>

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

std::string sha256(const std::string& input) {
    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const BYTE*>(input.c_str()), input.size());
    sha256_final(&ctx, hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::string PasswordManager::hashPassword(const std::string& password) {
    std::string salt = "RewardWallet";
    std::string salted = password + salt;
    return sha256(salted);
}

// Xác thực mật khẩu
bool PasswordManager::verifyPassword(const std::string& inputPassword, const std::string& storedHash) {
    std::string inputHash = hashPassword(inputPassword);
    return inputHash == storedHash;
}

// Tạo mật khẩu ngẫu nhiên với độ dài chỉ định
std::string PasswordManager::generateRandomPassword(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, charset.size() - 1);
    
    std::string password;
    for (int i = 0; i < length; ++i) {
        password += charset[distribution(generator)];
    }
    
    return password;
}