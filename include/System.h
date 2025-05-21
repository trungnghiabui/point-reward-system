#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>

class UserManager;
class AuthenticationManager;

class System {
private:
    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<AuthenticationManager> authManager;

    std::string currentUsername; // Người dùng hiện tại đang đăng nhập

    bool running; // Trạng thái chạy của hệ thống
    
    // Menu chức năng
    void showMainMenu();
    void showUserMenu();
    void showAdminMenu();

    // Xử lý đăng nhập, đăng ký
    void loginProcess();
    void registerProcess();

    // Xử lý chức năng admin
    void userManagementMenu();
    void createUserProcess();
    void listAllUsers();
    void searchUserProcess();
    void updateUserProfileByAdmin();
    
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