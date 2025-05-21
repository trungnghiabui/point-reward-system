#include "../include/OTPManager.h"
#include "../include/Transaction.h"
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>

OTPManager::OTPManager() {
    // Constructor
}

// Tạo OTP cơ bản
std::string OTPManager::generateOTP(const std::string& username) {
    // Tạo mã OTP 6 chữ số
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(100000, 999999);
    
    std::string otp = std::to_string(distribution(generator));
    
    // Lưu OTP vào store với thời gian hết hạn
    time_t expiryTime = time(nullptr) + OTP_VALIDITY_PERIOD;
    otpStore[username] = std::make_pair(otp, expiryTime);
    
    // Xóa mục đích OTP cũ nếu có
    if (otpPurpose.find(username) != otpPurpose.end()) {
        otpPurpose.erase(username);
    }
    
    return otp;
}

// Xác thực OTP
bool OTPManager::verifyOTP(const std::string& username, const std::string& inputOTP) {
    // Kiểm tra xem có OTP cho username không
    if (otpStore.find(username) == otpStore.end()) {
        std::cout << "Không có mã OTP cho người dùng này!" << std::endl;
        return false;
    }
    
    auto& [storedOTP, expiryTime] = otpStore[username];
    
    // Kiểm tra OTP còn hiệu lực không
    if (time(nullptr) > expiryTime) {
        std::cout << "Mã OTP đã hết hạn!" << std::endl;
        otpStore.erase(username);
        return false;
    }
    
    // Kiểm tra OTP có đúng không
    if (inputOTP != storedOTP) {
        std::cout << "Mã OTP không đúng!" << std::endl;
        return false;
    }
    
    // OTP đúng, xóa khỏi store
    otpStore.erase(username);
    return true;
}

// Mô phỏng gửi OTP (trong thực tế sẽ gửi qua SMS/email)
void OTPManager::sendOTP(const std::string& username, const std::string& otp) {
    std::cout << "\n---------- [GỬI OTP] ----------" << std::endl;
    std::cout << "Gửi đến người dùng: " << username << std::endl;
    std::cout << "Mã OTP của bạn là: " << otp << std::endl;
    std::cout << "Mã có hiệu lực trong " << OTP_VALIDITY_PERIOD / 60 << " phút." << std::endl;
    std::cout << "--------------------------------\n" << std::endl;
}

// Xóa OTP đã hết hạn
void OTPManager::cleanupExpiredOTPs() {
    time_t now = time(nullptr);
    
    auto it = otpStore.begin();
    while (it != otpStore.end()) {
        if (now > it->second.second) {
            it = otpStore.erase(it);
        } else {
            ++it;
        }
    }
}

// Sinh OTP cho xác nhận thay đổi thông tin
std::string OTPManager::generateInfoUpdateOTP(const std::string& username, const std::string& changeDescription) {
    std::string otp = generateOTP(username);
    otpPurpose[username] = "info_update";
    return otp;
}

// Xác thực OTP cho thay đổi thông tin
bool OTPManager::verifyInfoUpdateOTP(const std::string& username, const std::string& inputOTP) {
    if (otpPurpose.find(username) == otpPurpose.end() || otpPurpose[username] != "info_update") {
        std::cout << "Không có yêu cầu thay đổi thông tin nào được xác thực!" << std::endl;
        return false;
    }
    
    bool result = verifyOTP(username, inputOTP);
    
    if (result) {
        otpPurpose.erase(username);
    }
    
    return result;
}

// Mô phỏng gửi OTP kèm theo mô tả thay đổi
void OTPManager::sendInfoUpdateOTP(const std::string& username, const std::string& otp, const std::string& changeDescription) {
    std::cout << "\n---------- [GỬI OTP XÁC NHẬN THAY ĐỔI THÔNG TIN] ----------" << std::endl;
    std::cout << "Gửi đến người dùng: " << username << std::endl;
    std::cout << "Có yêu cầu thay đổi thông tin của bạn:" << std::endl;
    std::cout << changeDescription << std::endl;
    std::cout << "Mã OTP của bạn là: " << otp << std::endl;
    std::cout << "Mã có hiệu lực trong " << OTP_VALIDITY_PERIOD / 60 << " phút." << std::endl;
    std::cout << "-------------------------------------------------------\n" << std::endl;
}

// Sinh OTP cho xác nhận giao dịch
std::string OTPManager::generateTransactionOTP(const std::string& username, const Transaction& transaction) {
    std::string otp = generateOTP(username);
    otpPurpose[username] = "transaction";
    return otp;
}

// Xác thực OTP cho giao dịch
bool OTPManager::verifyTransactionOTP(const std::string& username, const std::string& inputOTP) {
    if (otpPurpose.find(username) == otpPurpose.end() || otpPurpose[username] != "transaction") {
        std::cout << "Không có giao dịch nào cần xác thực!" << std::endl;
        return false;
    }
    
    bool result = verifyOTP(username, inputOTP);
    
    if (result) {
        otpPurpose.erase(username);
    }
    
    return result;
}

// Mô phỏng gửi OTP kèm theo thông tin giao dịch
void OTPManager::sendTransactionOTP(const std::string& username, const std::string& otp, const Transaction& transaction) {
    std::cout << "\n---------- [GỬI OTP XÁC NHẬN GIAO DỊCH] ----------" << std::endl;
    std::cout << "Gửi đến người dùng: " << username << std::endl;
    std::cout << "Có yêu cầu giao dịch:" << std::endl;
    std::cout << "- Mã giao dịch: " << transaction.getTransactionId() << std::endl;
    std::cout << "- Từ ví: " << transaction.getSourceWalletId() << std::endl;
    std::cout << "- Đến ví: " << transaction.getDestinationWalletId() << std::endl;
    std::cout << "- Số điểm: " << transaction.getAmount() << std::endl;
    std::cout << "- Mô tả: " << transaction.getDescription() << std::endl;
    std::cout << "Mã OTP của bạn là: " << otp << std::endl;
    std::cout << "Mã có hiệu lực trong " << OTP_VALIDITY_PERIOD / 60 << " phút." << std::endl;
    std::cout << "-------------------------------------------------------\n" << std::endl;
}
