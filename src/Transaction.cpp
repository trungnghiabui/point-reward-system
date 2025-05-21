#include "../include/Transaction.h"
#include <random>
#include <sstream>
#include <iomanip>

Transaction::Transaction() : amount(0), timestamp(time(nullptr)), status(Status::PENDING) {
    // Tạo ID giao dịch
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(10000000, 99999999);
    
    std::stringstream ss;
    ss << "TX" << distrib(gen);
    transactionId = ss.str();
}

Transaction::Transaction(const std::string& sourceWalletId, 
                         const std::string& destinationWalletId,
                         int amount,
                         const std::string& description) 
    : sourceWalletId(sourceWalletId),
      destinationWalletId(destinationWalletId),
      amount(amount),
      timestamp(time(nullptr)),
      status(Status::PENDING),
      description(description) {
    
    // Tạo ID giao dịch
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(10000000, 99999999);
    
    std::stringstream ss;
    ss << "TX" << distrib(gen);
    transactionId = ss.str();
}

// Getters
std::string Transaction::getTransactionId() const {
    return transactionId;
}

std::string Transaction::getSourceWalletId() const {
    return sourceWalletId;
}

std::string Transaction::getDestinationWalletId() const {
    return destinationWalletId;
}

int Transaction::getAmount() const {
    return amount;
}

time_t Transaction::getTimestamp() const {
    return timestamp;
}

Transaction::Status Transaction::getStatus() const {
    return status;
}

std::string Transaction::getDescription() const {
    return description;
}

// Setters (chỉ dành cho TransactionManager)
void Transaction::setStatus(Status newStatus) {
    status = newStatus;
}

// Chuyển đổi Status thành chuỗi
std::string Transaction::statusToString(Status status) {
    switch (status) {
        case Status::PENDING:
            return "Đang chờ xử lý";
        case Status::COMPLETED:
            return "Hoàn thành";
        case Status::FAILED:
            return "Thất bại";
        case Status::CANCELED:
            return "Đã hủy";
        default:
            return "Không xác định";
    }
}

// Chuỗi hóa để lưu trữ
std::string Transaction::serialize() const {
    std::stringstream ss;
    ss << transactionId << "\n";
    ss << sourceWalletId << "\n";
    ss << destinationWalletId << "\n";
    ss << amount << "\n";
    ss << timestamp << "\n";
    ss << static_cast<int>(status) << "\n";
    ss << description << "\n";
    
    return ss.str();
}

Transaction Transaction::deserialize(const std::string& data) {
    std::istringstream iss(data);
    Transaction transaction;
    
    std::getline(iss, transaction.transactionId);
    std::getline(iss, transaction.sourceWalletId);
    std::getline(iss, transaction.destinationWalletId);
    
    std::string temp;
    std::getline(iss, temp);
    transaction.amount = std::stoi(temp);
    
    std::getline(iss, temp);
    transaction.timestamp = std::stol(temp);
    
    std::getline(iss, temp);
    transaction.status = static_cast<Status>(std::stoi(temp));
    
    std::getline(iss, transaction.description);
    
    return transaction;
}