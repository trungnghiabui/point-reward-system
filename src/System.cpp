#include "../include/System.h"
#include "../include/UserManager.h"
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
    userManager = std::make_unique<UserManager>("users.dat");
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

        switch (choice) {
            case 1:
                // TODO : login
                break;
            case 2:
                registerProcess();
                break;
            case 0:
                running = false;
                std::cout << "Đang thoát khỏi hệ thống..." << std::endl;
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
                break;
        }
    } while (choice != 0 && running);
}

void System::registerProcess() {
    std::string username, password, confirmPassword, fullName, email, phoneNumber, address;
    
    std::cout << "\n========== ĐĂNG KÝ TÀI KHOẢN ==========\n" << std::endl;
    
    std::cout << "Tên đăng nhập: ";
    std::getline(std::cin, username);
    
    std::cout << "Mật khẩu: ";
    std::getline(std::cin, password);
    
    std::cout << "Xác nhận mật khẩu: ";
    std::getline(std::cin, confirmPassword);
    
    if (password != confirmPassword) {
        std::cout << "Mật khẩu không khớp!" << std::endl;
        return;
    }
    
    std::cout << "Họ và tên: ";
    std::getline(std::cin, fullName);
    
    std::cout << "Email: ";
    std::getline(std::cin, email);
    
    std::cout << "Số điện thoại: ";
    std::getline(std::cin, phoneNumber);
    
    std::cout << "Địa chỉ: ";
    std::getline(std::cin, address);
    
    if (userManager->registerUser(username, password, fullName, email, phoneNumber, address)) {
        std::cout << "Đăng ký thành công! Vui lòng đăng nhập để tiếp tục." << std::endl;
    }
}

void System::shutdown() {
    // Todo: gracefully shutdown the system
    std::cout << "Đã tắt hệ thống an toàn." << std::endl;
}