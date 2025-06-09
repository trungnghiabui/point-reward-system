#include "../include/Wallet.h"
#include <random>
#include <sstream>
#include <iomanip>

Wallet::Wallet() : balance(0), isMasterWallet(false) {
}

Wallet::Wallet(const std::string& ownerUsername, bool isMaster) 
    : ownerUsername(ownerUsername), 
      balance(0), 
      isMasterWallet(isMaster) {
    
    // Tạo ID ví
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1000000, 9999999);
    
    std::stringstream ss;
    if (isMasterWallet) {
        ss << "MASTER" << distrib(gen);
    } else {
        ss << "W" << distrib(gen);
    }
    walletId = ss.str();
    
    // Nếu là ví tổng, khởi tạo số dư lớn
    if (isMasterWallet) {
        balance = 1000000; // Khởi tạo ví tổng với 1 triệu điểm
    }
}

// Getters
std::string Wallet::getWalletId() const {
    return walletId;
}

std::string Wallet::getOwnerUsername() const {
    return ownerUsername;
}

int Wallet::getBalance() const {
    return balance;
}

bool Wallet::isMaster() const {
    return isMasterWallet;
}

// Chuỗi hóa để lưu trữ
std::string Wallet::serialize() const {
    std::stringstream ss;
    ss << walletId << "\n";
    ss << ownerUsername << "\n";
    ss << balance << "\n";
    ss << (isMasterWallet ? "1" : "0") << "\n";
    
    return ss.str();
}

Wallet Wallet::deserialize(const std::string& data) {
    std::istringstream iss(data);
    Wallet wallet;
    
    std::getline(iss, wallet.walletId);
    std::getline(iss, wallet.ownerUsername);
    
    std::string temp;
    std::getline(iss, temp);
    wallet.balance = std::stoi(temp);
    
    std::getline(iss, temp);
    wallet.isMasterWallet = (temp == "1");
    
    return wallet;
}
