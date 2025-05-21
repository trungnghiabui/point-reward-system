#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>

class UserManager;
class WalletManager;
class TransactionManager;
class AuthenticationManager;

class System {
private:
    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<WalletManager> walletManager;
    std::unique_ptr<TransactionManager> transactionManager;
    std::unique_ptr<AuthenticationManager> authManager;

    std::string currentUsername; // Người dùng hiện tại đang đăng nhập

    bool running; // Trạng thái chạy của hệ thống
    
    // Menu chức năng
    void showMainMenu();
    void showUserMenu();

    // Xử lý đăng nhập, đăng ký
    void loginProcess();
    void registerProcess();
    
    // Xử lý chức năng người dùng
    void walletManagementMenu();
    void transferPoints();
    void viewTransactionHistory();

public:
    System();
    ~System();
    
    // Khởi động và đóng hệ thống
    void initialize();
    void run();
    void shutdown();

    // Kiểm tra và tạo dữ liệu mặc định
    void checkAndCreateDefaultData();
};



#endif // SYSTEM_H