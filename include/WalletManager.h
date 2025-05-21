#ifndef WALLET_MANAGER_H
#define WALLET_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Wallet.h"

class TransactionManager;

class WalletManager {
private:
    std::unordered_map<std::string, Wallet> wallets;      // walletId -> Wallet
    std::unordered_map<std::string, std::string> userWallets; // username -> walletId
    std::string dataFilePath;                              // Đường dẫn file lưu dữ liệu
    
public:
    std::string masterWalletId;                            // ID của ví tổng
    
public:
    WalletManager(const std::string& dataFilePath = "wallets.dat");
    ~WalletManager();
    
    // Quản lý dữ liệu
    void loadData();
    void saveData() const;
    
    // Tạo và quản lý ví
    Wallet* createWallet(const std::string& ownerUsername);
    Wallet* createMasterWallet();
    Wallet* getWallet(const std::string& walletId);
    Wallet* getUserWallet(const std::string& username);
    bool walletExists(const std::string& walletId) const;
    bool userHasWallet(const std::string& username) const;
    
    // Chỉ cho TransactionManager truy cập
    friend class TransactionManager;
};

#endif // WALLET_MANAGER_H