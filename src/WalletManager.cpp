#include "../include/WalletManager.h"
#include <fstream>
#include <iostream>

WalletManager::WalletManager(const std::string& dataFilePath) 
    : dataFilePath(dataFilePath) {
    loadData();
}

WalletManager::~WalletManager() {
    saveData();
}

// Quản lý dữ liệu
void WalletManager::loadData() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "Không thể mở tệp dữ liệu ví. Tạo mới dữ liệu." << std::endl;
        return;
    }
    
    wallets.clear();
    userWallets.clear();
    
    std::string line;
    std::string walletData;
    
    while (std::getline(file, line)) {
        if (line == "#WALLET") {
            walletData.clear();
            
            // Đọc dữ liệu ví cho đến khi gặp dấu phân cách
            while (std::getline(file, line) && line != "#END") {
                walletData += line + "\n";
            }
            
            if (!walletData.empty()) {
                Wallet wallet = Wallet::deserialize(walletData);
                wallets[wallet.getWalletId()] = wallet;
                
                if (wallet.isMaster()) {
                    masterWalletId = wallet.getWalletId();
                } else {
                    userWallets[wallet.getOwnerUsername()] = wallet.getWalletId();
                }
            }
        }
    }
    
    file.close();
}

void WalletManager::saveData() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cerr << "Không thể mở tệp dữ liệu ví để lưu!" << std::endl;
        return;
    }
    
    for (const auto& [walletId, wallet] : wallets) {
        file << "#WALLET" << std::endl;
        file << wallet.serialize();
        file << "#END" << std::endl;
    }
    
    file.close();
}

// Tạo và quản lý ví
Wallet* WalletManager::createWallet(const std::string& ownerUsername) {
    // Kiểm tra xem người dùng đã có ví chưa
    if (userHasWallet(ownerUsername)) {
        std::cout << "Người dùng " << ownerUsername << " đã có ví!" << std::endl;
        return getUserWallet(ownerUsername);
    }
    
    // Tạo ví mới
    Wallet newWallet(ownerUsername, false);
    std::string walletId = newWallet.getWalletId();
    
    // Lưu ví vào hệ thống
    wallets[walletId] = newWallet;
    userWallets[ownerUsername] = walletId;
    
    std::cout << "Đã tạo ví " << walletId << " cho người dùng " << ownerUsername << std::endl;
    
    // Lưu dữ liệu
    saveData();
    
    return &wallets[walletId];
}

Wallet* WalletManager::createMasterWallet() {
    // Kiểm tra xem đã có ví tổng chưa
    if (!masterWalletId.empty()) {
        std::cout << "Đã tồn tại ví tổng!" << std::endl;
        return &wallets[masterWalletId];
    }
    
    // Tạo ví tổng mới
    Wallet masterWallet("admin", true);
    masterWalletId = masterWallet.getWalletId();
    
    // Lưu ví vào hệ thống
    wallets[masterWalletId] = masterWallet;
    
    std::cout << "Đã tạo ví tổng " << masterWalletId << std::endl;
    
    // Lưu dữ liệu
    saveData();
    
    return &wallets[masterWalletId];
}

Wallet* WalletManager::getWallet(const std::string& walletId) {
    if (walletExists(walletId)) {
        return &wallets[walletId];
    }
    return nullptr;
}

Wallet* WalletManager::getUserWallet(const std::string& username) {
    if (userHasWallet(username)) {
        std::string walletId = userWallets[username];
        return &wallets[walletId];
    }
    return nullptr;
}

bool WalletManager::walletExists(const std::string& walletId) const {
    return wallets.find(walletId) != wallets.end();
}

bool WalletManager::userHasWallet(const std::string& username) const {
    return userWallets.find(username) != userWallets.end();
}