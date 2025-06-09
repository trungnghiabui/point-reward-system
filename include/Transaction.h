#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

class Transaction {
public:
    enum class Status {
        PENDING,      // Chờ xác nhận OTP
        COMPLETED,    // Hoàn thành
        FAILED,       // Thất bại
        CANCELED      // Bị hủy
    };
    
private:
    std::string transactionId;         // Mã giao dịch duy nhất
    std::string sourceWalletId;        // ID ví nguồn
    std::string destinationWalletId;   // ID ví đích
    int amount;                        // Số điểm chuyển
    time_t timestamp;                  // Thời gian giao dịch
    Status status;                     // Trạng thái giao dịch
    std::string description;           // Mô tả giao dịch
    
public:
    Transaction();
    Transaction(const std::string& sourceWalletId, 
               const std::string& destinationWalletId,
               int amount,
               const std::string& description = "");
    
    // Getters
    std::string getTransactionId() const;
    std::string getSourceWalletId() const;
    std::string getDestinationWalletId() const;
    int getAmount() const;
    time_t getTimestamp() const;
    Status getStatus() const;
    std::string getDescription() const;
    
    // Setters (chỉ dành cho TransactionManager)
    void setStatus(Status newStatus);
    
    // Chuyển đổi Status thành chuỗi
    static std::string statusToString(Status status);
    
    // Chuỗi hóa để lưu trữ
    std::string serialize() const;
    static Transaction deserialize(const std::string& data);
    
    friend class TransactionManager;
};

#endif // TRANSACTION_H