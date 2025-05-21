#ifndef WALLET_H
#define WALLET_H

#include <string>

class TransactionManager;

class Wallet {
private:
    std::string walletId;          // Mã định danh duy nhất của ví
    std::string ownerUsername;     // Tên người dùng sở hữu ví
    int balance;                   // Số dư hiện tại
    bool isMasterWallet;           // Đánh dấu ví tổng
    
public:
    Wallet();
    Wallet(const std::string& ownerUsername, bool isMaster = false);
    
    // Getters
    std::string getWalletId() const;
    std::string getOwnerUsername() const;
    int getBalance() const;
    bool isMaster() const;
    
    // Chuỗi hóa để lưu trữ
    std::string serialize() const;
    static Wallet deserialize(const std::string& data);
    
    // Chỉ cho TransactionManager truy cập, thay đổi số dư
    friend class TransactionManager;
};

#endif // WALLET_H