#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>


class System {
private:
    bool running; // Trạng thái chạy của hệ thống
    
    // Menu chức năng
    void showMainMenu();
    
public:
    System();
    ~System();
    
    // Khởi động và đóng hệ thống
    void initialize();
    void run();
    void shutdown();
};

#endif // SYSTEM_H