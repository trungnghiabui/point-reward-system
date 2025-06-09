#ifndef OTP_MANAGER_H
#define OTP_MANAGER_H

#include <string>
#include <unordered_map>
#include <utility>
#include <ctime>

// Forward declaration
class Transaction;

class OTPManager {
private:
    std::unordered_map<std::string, std::pair<std::string, time_t>> otpStore; // username -> {otp, expiryTime}
    const int OTP_VALIDITY_PERIOD = 300; // 5 phút, tính bằng giây
    
    // Thêm ánh xạ để lưu loại OTP (xác nhận thay đổi thông tin, đăng nhập...)
    std::unordered_map<std::string, std::string> otpPurpose; // username -> purpose
    
public:
    OTPManager();
    
    // Tạo OTP cơ bản
    std::string generateOTP(const std::string& username);
    
    // Xác thực OTP
    bool verifyOTP(const std::string& username, const std::string& inputOTP);
    
    // Mô phỏng gửi OTP (trong thực tế sẽ gửi qua SMS/email)
    void sendOTP(const std::string& username, const std::string& otp);
    
    // Xóa OTP đã hết hạn
    void cleanupExpiredOTPs();
    
    // Sinh OTP cho xác nhận thay đổi thông tin
    std::string generateInfoUpdateOTP(const std::string& username, const std::string& changeDescription);
    
    // Xác thực OTP cho thay đổi thông tin
    bool verifyInfoUpdateOTP(const std::string& username, const std::string& inputOTP);
    
    // Mô phỏng gửi OTP kèm theo mô tả thay đổi
    void sendInfoUpdateOTP(const std::string& username, const std::string& otp, const std::string& changeDescription);
    
    // Sinh OTP cho xác nhận giao dịch
    std::string generateTransactionOTP(const std::string& username, const Transaction& transaction);
    
    // Xác thực OTP cho giao dịch
    bool verifyTransactionOTP(const std::string& username, const std::string& inputOTP);
    
    // Mô phỏng gửi OTP kèm theo thông tin giao dịch
    void sendTransactionOTP(const std::string& username, const std::string& otp, const Transaction& transaction);
};

#endif // OTP_MANAGER_H