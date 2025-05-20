#include <iostream>
#include "../include/System.h"

int main() {
    std::cout << "Khởi động Hệ thống Quản lý Ví Điểm Thưởng..." << std::endl;
    
    System system;
    system.initialize();
    system.run();
    system.shutdown();
    
    std::cout << "Hệ thống đã đóng. Tạm biệt!" << std::endl;
    return 0;
}