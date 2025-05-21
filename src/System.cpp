#include "../include/System.h"
#include "../include/UserManager.h"
#include "AuthenticationManager.h"
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
    authManager = std::make_unique<AuthenticationManager>(*userManager);

    // Kiểm tra và tạo dữ liệu mặc định
    checkAndCreateDefaultData();
    std::cout << "Hệ thống đã sẵn sàng." << std::endl;
}

void System::checkAndCreateDefaultData() {
    // Kiểm tra xem đã có admin chưa
    bool hasAdmin = false;
    auto users = userManager->getAllUsers();
    
    for (const auto& user : users) {
        if (user.getIsAdmin()) {
            hasAdmin = true;
            break;
        }
    }
    
    // Nếu chưa có admin, tạo tài khoản admin mặc định
    if (!hasAdmin && users.empty()) {
        std::cout << "Tạo tài khoản admin mặc định..." << std::endl;
        
        std::string adminUsername = "admin";
        std::string adminPassword = "Admin@123";
        std::string adminFullName = "Administrator";
        std::string adminEmail = "admin@example.com";
        std::string adminPhone = "0123456789";
        std::string adminAddress = "System";
        
        userManager->registerUser(adminUsername, adminPassword, adminFullName, 
                                adminEmail, adminPhone, adminAddress);
        
        // Đặt quyền admin
        User* admin = userManager->getUser(adminUsername);
        if (admin) {
            admin->setIsAdmin(true);
            userManager->saveData();
        }
        
        std::cout << "Đã tạo tài khoản admin:" << std::endl;
        std::cout << "- Tên đăng nhập: " << adminUsername << std::endl;
        std::cout << "- Mật khẩu: " << adminPassword << std::endl;
    }
}


void System::run() {
    running = true;
    while (running) {
        // Nếu chưa đăng nhập, hiển thị menu chính
        if (currentUsername.empty()) {
            showMainMenu();
        } else {
            // Kiểm tra xem người dùng có phải admin không
            User* user = userManager->getUser(currentUsername);
            
            if (user && user->getIsAdmin()) {
                showAdminMenu();
            } else {
                showUserMenu();
            }
        }
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
                loginProcess();
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
    } while (choice != 0 && currentUsername.empty() && running);
}

void System::showUserMenu() {
    int choice = 0;
    do {
        
        std::cout << "\n========== MENU NGƯỜI DÙNG ==========\n" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
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

void System::showAdminMenu() {
    int choice = 0;
    
    do {
        std::cout << "\n========== MENU QUẢN TRỊ VIÊN ==========\n" << std::endl;
        std::cout << "1. Quản lý người dùng" << std::endl;
        std::cout << "2. Đăng xuất" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                userManagementMenu();
                break;
            case 0:
                running = false;
                std::cout << "Đang thoát khỏi hệ thống..." << std::endl;
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
                break;
        }
    } while (choice != 0 && choice != 9 && running);
}

void System::loginProcess() {
    std::string username, password;
    
    std::cout << "\n========== ĐĂNG NHẬP ==========\n" << std::endl;
    std::cout << "Tên đăng nhập: ";
    std::getline(std::cin, username);
    
    std::cout << "Mật khẩu: ";
    std::getline(std::cin, password);
    
    if (authManager->login(username, password)) {
        currentUsername = username;
        std::cout << "Đăng nhập thành công!" << std::endl;
    }
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

// Xử lý chức năng admin
void System::userManagementMenu() {
    int choice = 0;
    
    do {
        std::cout << "\n========== QUẢN LÝ NGƯỜI DÙNG ==========\n" << std::endl;
        std::cout << "1. Tạo tài khoản mới" << std::endl;
        std::cout << "2. Xem danh sách người dùng" << std::endl;
        std::cout << "3. Tìm kiếm người dùng" << std::endl;
        std::cout << "4. Cập nhật thông tin người dùng" << std::endl;
        std::cout << "0. Quay lại" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                createUserProcess();
                break;
            case 2:
                listAllUsers();
                break;
            case 3:
                searchUserProcess();
                break;
            case 0:
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
                break;
        }
    } while (choice != 0);
}

void System::createUserProcess() {
    std::string username, fullName, email, phoneNumber, address;
    
    std::cout << "\n========== TẠO TÀI KHOẢN MỚI ==========\n" << std::endl;
    
    std::cout << "Tên đăng nhập: ";
    std::getline(std::cin, username);
    
    std::cout << "Họ và tên: ";
    std::getline(std::cin, fullName);
    
    std::cout << "Email: ";
    std::getline(std::cin, email);
    
    std::cout << "Số điện thoại: ";
    std::getline(std::cin, phoneNumber);
    
    std::cout << "Địa chỉ: ";
    std::getline(std::cin, address);
    
    if (userManager->createUserByAdmin(currentUsername, username, fullName, email, phoneNumber, address)) {
        std::cout << "Đã tạo tài khoản thành công!" << std::endl;
    }
}

void System::listAllUsers() {
    auto users = userManager->getAllUsers();
    
    std::cout << "\n========== DANH SÁCH NGƯỜI DÙNG ==========\n" << std::endl;
    
    if (users.empty()) {
        std::cout << "Không có người dùng nào." << std::endl;
        std::cout << "\nNhấn Enter để tiếp tục...";
        std::cin.get();
        return;
    }
    
    std::cout << std::left << std::setw(15) << "Tên đăng nhập"
              << std::setw(20) << "Họ tên"
              << std::setw(25) << "Email"
              << std::setw(15) << "Điện thoại"
              << std::setw(10) << "Admin" << std::endl;
    
    std::cout << std::string(85, '-') << std::endl;
    
    for (const auto& user : users) {
        std::cout << std::left << std::setw(15) << user.getUsername()
                  << std::setw(20) << user.getFullName()
                  << std::setw(25) << user.getEmail()
                  << std::setw(15) << user.getPhoneNumber()
                  << std::setw(10) << (user.getIsAdmin() ? "Có" : "Không") << std::endl;
    }
    
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::cin.get();
}

void System::searchUserProcess() {
    std::string searchTerm;
    
    std::cout << "\n========== TÌM KIẾM NGƯỜI DÙNG ==========\n" << std::endl;
    
    std::cout << "Nhập từ khóa tìm kiếm: ";
    std::getline(std::cin, searchTerm);
    
    auto users = userManager->searchUsers(searchTerm);
    
    std::cout << "\nKết quả tìm kiếm (" << users.size() << " người dùng):\n" << std::endl;
    
    if (users.empty()) {
        std::cout << "Không tìm thấy người dùng nào." << std::endl;
        std::cout << "\nNhấn Enter để tiếp tục...";
        std::cin.get();
        return;
    }
    
    std::cout << std::left << std::setw(15) << "Tên đăng nhập"
              << std::setw(20) << "Họ tên"
              << std::setw(25) << "Email"
              << std::setw(15) << "Điện thoại" << std::endl;
    
    std::cout << std::string(75, '-') << std::endl;
    
    for (const auto& user : users) {
        std::cout << std::left << std::setw(15) << user.getUsername()
                  << std::setw(20) << user.getFullName()
                  << std::setw(25) << user.getEmail()
                  << std::setw(15) << user.getPhoneNumber() << std::endl;
    }
    
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::cin.get();
}


void System::shutdown() {
    // Todo: gracefully shutdown the system
    std::cout << "Đã tắt hệ thống an toàn." << std::endl;
}