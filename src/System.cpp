#include "../include/System.h"
#include <iostream>
#include <limits>
#include <iomanip>

System::System() : running(false) {
}

System::~System() {
    shutdown();
}

void System::initialize() {
    std::cout << "\n===== KHỞI TẠO HỆ THỐNG =====\n" << std::endl;
    // Todo: Khởi tạo các thành phần của hệ thống
    std::cout << "Hệ thống đã sẵn sàng." << std::endl;
}

void System::run() {
    running = true;
    while (running) {
        showMainMenu();
    }
}

void System::showMainMenu() {
    int choice = 0;
    
    do {
        std::cout << "\n========== HỆ THỐNG QUẢN LÝ VÍ ĐIỂM THƯỞNG ==========\n" << std::endl;
        std::cout << "1. Đăng nhập" << std::endl;
        std::cout << "2. Đăng ký" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (choice != 0 && running);
}

void System::shutdown() {
    // Todo: gracefully shutdown the system
    std::cout << "Đã tắt hệ thống an toàn." << std::endl;
}