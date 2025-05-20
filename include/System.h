#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>

class UserManager;


class System {
private:
    std::unique_ptr<UserManager> userManager;

    std::string currentUsername; // Người dùng hiện tại đang đăng nhập

    bool running; // Trạng thái chạy của hệ thống
    
    // Menu chức năng
    void showMainMenu();
    void showUserMenu();

    // Xử lý đăng nhập, đăng ký
    void loginProcess();
    void registerProcess();
    
public:
    System();
    ~System();
    
    // Khởi động và đóng hệ thống
    void initialize();
    void run();
    void shutdown();
};



#endif // SYSTEM_H