#include "../include/TransactionManager.h"
#include "../include/WalletManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

TransactionManager::TransactionManager(WalletManager& walletManager, 
                                     const std::string& dataFilePath) 
    : walletManager(walletManager), 
      dataFilePath(dataFilePath) {
    loadData();
}

TransactionManager::~TransactionManager() {
    saveData();
}

// Quản lý dữ liệu
void TransactionManager::loadData() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "Không thể mở tệp dữ liệu giao dịch. Tạo mới dữ liệu." << std::endl;
        return;
    }
    
    transactions.clear();
    
    std::string line;
    std::string transactionData;
    
    while (std::getline(file, line)) {
        if (line == "#TRANSACTION") {
            transactionData.clear();
            
            // Đọc dữ liệu giao dịch cho đến khi gặp dấu phân cách
            while (std::getline(file, line) && line != "#END") {
                transactionData += line + "\n";
            }
            
            if (!transactionData.empty()) {
                Transaction transaction = Transaction::deserialize(transactionData);
                transactions.push_back(transaction);
            }
        }
    }
    
    file.close();
}

void TransactionManager::saveData() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cerr << "Không thể mở tệp dữ liệu giao dịch để lưu!" << std::endl;
        return;
    }
    
    for (const auto& transaction : transactions) {
        file << "#TRANSACTION" << std::endl;
        file << transaction.serialize();
        file << "#END" << std::endl;
    }
    
    file.close();
}

// Khởi tạo giao dịch
bool TransactionManager::initiateTransaction(const std::string& sourceUsername, 
                                           const std::string& destinationWalletId,
                                           int amount,
                                           const std::string& description) {
    // Kiểm tra ví nguồn
    if (!walletManager.userHasWallet(sourceUsername)) {
        std::cout << "Người dùng " << sourceUsername << " không có ví!" << std::endl;
        return false;
    }
    
    // Kiểm tra ví đích
    if (!walletManager.walletExists(destinationWalletId)) {
        std::cout << "Ví đích " << destinationWalletId << " không tồn tại!" << std::endl;
        return false;
    }
    
    // Lấy ví nguồn
    Wallet* sourceWallet = walletManager.getUserWallet(sourceUsername);
    
    // Kiểm tra số dư
    if (sourceWallet->getBalance() < amount) {
        std::cout << "Số dư không đủ! Số dư hiện tại: " << sourceWallet->getBalance() << ", Cần: " << amount << std::endl;
        return false;
    }
    
    // Tạo giao dịch mới
    Transaction transaction(sourceWallet->getWalletId(), destinationWalletId, amount, description);
    
    return executeTransaction(transaction);
}

// Thực hiện giao dịch
bool TransactionManager::executeTransaction(Transaction& transaction) {
    // Lấy ví nguồn và đích
    Wallet* sourceWallet = walletManager.getWallet(transaction.getSourceWalletId());
    Wallet* destWallet = walletManager.getWallet(transaction.getDestinationWalletId());
    
    // Kiểm tra ví tồn tại
    if (!sourceWallet || !destWallet) {
        transaction.setStatus(Transaction::Status::FAILED);
        transactions.push_back(transaction);
        std::cout << "Ví nguồn hoặc ví đích không tồn tại!" << std::endl;
        return false;
    }
    
    // Kiểm tra số dư
    if (sourceWallet->balance < transaction.getAmount()) {
        transaction.setStatus(Transaction::Status::FAILED);
        transactions.push_back(transaction);
        std::cout << "Số dư không đủ! Số dư hiện tại: " << sourceWallet->balance 
                  << ", Cần: " << transaction.getAmount() << std::endl;
        return false;
    }
    
    // Backup số dư hiện tại để rollback nếu cần
    int sourceBalance = sourceWallet->balance;
    int destBalance = destWallet->balance;
    
    try {
        // Thực hiện giao dịch
        sourceWallet->balance -= transaction.getAmount();
        destWallet->balance += transaction.getAmount();
        
        // Cập nhật trạng thái
        transaction.setStatus(Transaction::Status::COMPLETED);
        transactions.push_back(transaction);
        
        // Lưu dữ liệu
        saveData();
        walletManager.saveData();
        
        std::cout << "Giao dịch hoàn thành thành công!" << std::endl;
        std::cout << "- Từ ví: " << transaction.getSourceWalletId() << std::endl;
        std::cout << "- Đến ví: " << transaction.getDestinationWalletId() << std::endl;
        std::cout << "- Số điểm: " << transaction.getAmount() << std::endl;
        
        return true;
    } 
    catch (...) {
        // Rollback nếu có lỗi
        sourceWallet->balance = sourceBalance;
        destWallet->balance = destBalance;
        transaction.setStatus(Transaction::Status::FAILED);
        transactions.push_back(transaction);
        
        std::cerr << "Có lỗi xảy ra trong quá trình thực hiện giao dịch!" << std::endl;
        return false;
    }
}

// Nạp điểm từ ví tổng cho người dùng (chỉ dành cho admin)
bool TransactionManager::addPointsFromMasterWallet(const std::string& adminUsername,
                                                 const std::string& targetUsername,
                                                 int amount,
                                                 const std::string& description) {
    // Kiểm tra ví tổng
    if (walletManager.masterWalletId.empty()) {
        std::cout << "Ví tổng không tồn tại!" << std::endl;
        return false;
    }
    
    // Kiểm tra ví người dùng đích
    if (!walletManager.userHasWallet(targetUsername)) {
        std::cout << "Người dùng " << targetUsername << " không có ví!" << std::endl;
        return false;
    }
    
    // Lấy ví tổng và ví đích
    Wallet* masterWallet = walletManager.getWallet(walletManager.masterWalletId);
    Wallet* targetWallet = walletManager.getUserWallet(targetUsername);
    
    // Kiểm tra số dư ví tổng
    if (masterWallet->balance < amount) {
        std::cout << "Số dư ví tổng không đủ!" << std::endl;
        return false;
    }
    
    // Tạo giao dịch mới
    Transaction transaction(masterWallet->getWalletId(), targetWallet->getWalletId(), 
                           amount, "Nạp điểm bởi " + adminUsername + ": " + description);
    
    // Cập nhật số dư
    masterWallet->balance -= amount;
    targetWallet->balance += amount;
    
    // Cập nhật trạng thái giao dịch
    transaction.setStatus(Transaction::Status::COMPLETED);
    transactions.push_back(transaction);
    
    // Lưu dữ liệu
    saveData();
    walletManager.saveData();
    
    std::cout << "Đã nạp " << amount << " điểm cho người dùng " << targetUsername << std::endl;
    return true;
}

// Lấy thông tin giao dịch
Transaction* TransactionManager::getTransaction(const std::string& transactionId) {
    for (auto& transaction : transactions) {
        if (transaction.getTransactionId() == transactionId) {
            return &transaction;
        }
    }
    return nullptr;
}

std::vector<Transaction> TransactionManager::getUserTransactions(const std::string& username) const {
    std::vector<Transaction> userTransactions;
    
    // Lấy ID ví của người dùng
    if (!walletManager.userHasWallet(username)) {
        return userTransactions;
    }
    
    std::string walletId = walletManager.userWallets.at(username);
    
    // Tìm các giao dịch liên quan
    for (const auto& transaction : transactions) {
        if (transaction.getSourceWalletId() == walletId || transaction.getDestinationWalletId() == walletId) {
            userTransactions.push_back(transaction);
        }
    }
    
    return userTransactions;
}

// Báo cáo giao dịch
int TransactionManager::getUserBalance(const std::string& username) const {
    if (!walletManager.userHasWallet(username)) {
        return 0;
    }
    
    const Wallet* wallet = walletManager.getUserWallet(username);
    return wallet->getBalance();
}

std::vector<Transaction> TransactionManager::getUserTransactionHistory(const std::string& username) const {
    return getUserTransactions(username);
}

// Lọc giao dịch theo các tiêu chí
std::vector<Transaction> TransactionManager::filterTransactions(const std::string& username, 
                                                              time_t startTime, 
                                                              time_t endTime, 
                                                              Transaction::Status status) const {
    std::vector<Transaction> filteredTransactions;
    
    auto userTransactions = getUserTransactions(username);
    
    for (const auto& transaction : userTransactions) {
        time_t timestamp = transaction.getTimestamp();
        if (timestamp >= startTime && timestamp <= endTime && transaction.getStatus() == status) {
            filteredTransactions.push_back(transaction);
        }
    }
    
    return filteredTransactions;
}

// Thống kê giao dịch
int TransactionManager::getTotalTransacted(const std::string& username) const {
    int total = 0;
    
    if (!walletManager.userHasWallet(username)) {
        return 0;
    }
    
    std::string walletId = walletManager.userWallets.at(username);
    
    for (const auto& transaction : transactions) {
        if ((transaction.getSourceWalletId() == walletId || transaction.getDestinationWalletId() == walletId) 
            && transaction.getStatus() == Transaction::Status::COMPLETED) {
            total += transaction.getAmount();
        }
    }
    
    return total;
}

int TransactionManager::getTotalReceived(const std::string& username) const {
    int total = 0;
    
    if (!walletManager.userHasWallet(username)) {
        return 0;
    }
    
    std::string walletId = walletManager.userWallets.at(username);
    
    for (const auto& transaction : transactions) {
        if (transaction.getDestinationWalletId() == walletId 
            && transaction.getStatus() == Transaction::Status::COMPLETED) {
            total += transaction.getAmount();
        }
    }
    
    return total;
}

int TransactionManager::getTotalSent(const std::string& username) const {
    int total = 0;
    
    if (!walletManager.userHasWallet(username)) {
        return 0;
    }
    
    std::string walletId = walletManager.userWallets.at(username);
    
    for (const auto& transaction : transactions) {
        if (transaction.getSourceWalletId() == walletId 
            && transaction.getStatus() == Transaction::Status::COMPLETED) {
            total += transaction.getAmount();
        }
    }
    
    return total;
}