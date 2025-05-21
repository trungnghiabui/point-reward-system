#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Transaction.h"

class WalletManager;
class OTPManager;

class TransactionManager {
private:
    WalletManager& walletManager;
    OTPManager& otpManager;
    std::vector<Transaction> transactions;
    std::string dataFilePath;
    
    // Giao dịch đang chờ xác thực OTP
    std::unordered_map<std::string, Transaction> pendingTransactions; // username -> Transaction
    
    // Thực hiện giao dịch sau khi đã xác thực OTP
    bool executeTransaction(Transaction& transaction);
    
public:
    TransactionManager(WalletManager& walletManager, 
                      OTPManager& otpManager,
                      const std::string& dataFilePath = "transactions.dat");
    ~TransactionManager();
    
    // Quản lý dữ liệu
    void loadData();
    void saveData() const;
    
    // Khởi tạo giao dịch (tạo và gửi OTP)
    bool initiateTransaction(const std::string& sourceUsername, 
                           const std::string& destinationWalletId,
                           int amount,
                           const std::string& description = "");
    
    // Xác nhận giao dịch bằng OTP
    bool confirmTransaction(const std::string& username, const std::string& otp);
    
    // Hủy giao dịch đang chờ
    bool cancelPendingTransaction(const std::string& username);
    
    // Nạp điểm từ ví tổng cho người dùng (chỉ dành cho admin)
    bool addPointsFromMasterWallet(const std::string& adminUsername,
                                 const std::string& targetUsername,
                                 int amount,
                                 const std::string& description = "");
    
    // Lấy thông tin giao dịch
    Transaction* getTransaction(const std::string& transactionId);
    std::vector<Transaction> getUserTransactions(const std::string& username) const;
    
    // Lấy giao dịch đang chờ của người dùng
    Transaction* getPendingTransaction(const std::string& username);
    
    // Kiểm tra người dùng có giao dịch đang chờ xử lý không
    bool hasPendingTransaction(const std::string& username) const;
    
    // Báo cáo giao dịch
    int getUserBalance(const std::string& username) const;
    std::vector<Transaction> getUserTransactionHistory(const std::string& username) const;
    
    // Lọc giao dịch theo các tiêu chí
    std::vector<Transaction> filterTransactions(const std::string& username, 
                                              time_t startTime, 
                                              time_t endTime, 
                                              Transaction::Status status = Transaction::Status::COMPLETED) const;
    
    // Thống kê giao dịch
    int getTotalTransacted(const std::string& username) const;
    int getTotalReceived(const std::string& username) const;
    int getTotalSent(const std::string& username) const;
};

#endif // TRANSACTION_MANAGER_H