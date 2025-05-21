#include "../include/System.h"
#include "../include/UserManager.h"
#include "../include/WalletManager.h"
#include "../include/TransactionManager.h"
#include "../include/AuthenticationManager.h"
#include "../include/OTPManager.h"
#include "../include/PasswordManager.h"
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
    
    // Khởi tạo các thành phần
    otpManager = std::make_unique<OTPManager>();
    userManager = std::make_unique<UserManager>("users.dat", *otpManager);
    walletManager = std::make_unique<WalletManager>("wallets.dat");
    transactionManager = std::make_unique<TransactionManager>(*walletManager, *otpManager, "transactions.dat");
    authManager = std::make_unique<AuthenticationManager>(*userManager, *otpManager);
    
    // Kiểm tra và tạo dữ liệu mặc định nếu cần
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
    
    // Kiểm tra và tạo ví tổng nếu chưa có
    if (walletManager->masterWalletId.empty()) {
        std::cout << "Tạo ví tổng hệ thống..." << std::endl;
        walletManager->createMasterWallet();
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

void System::shutdown() {
    if (!currentUsername.empty()) {
        authManager->logout(currentUsername);
        currentUsername.clear();
    }
    
    std::cout << "\nĐang lưu dữ liệu hệ thống..." << std::endl;
    
    // Các manager sẽ tự động lưu dữ liệu trong destructor
    otpManager.reset();
    userManager.reset();
    walletManager.reset();
    transactionManager.reset();
    authManager.reset();
    
    std::cout << "Đã tắt hệ thống an toàn." << std::endl;
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

void System::showAdminMenu() {
    int choice = 0;
    
    do {
        std::cout << "\n========== MENU QUẢN TRỊ VIÊN ==========\n" << std::endl;
        std::cout << "1. Quản lý người dùng" << std::endl;
        std::cout << "2. Quản lý ví cá nhân" << std::endl;
        std::cout << "3. Thay đổi mật khẩu" << std::endl;
        std::cout << "4. Cập nhật thông tin cá nhân" << std::endl;
        std::cout << "9. Đăng xuất" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                userManagementMenu();
                break;
            case 2:
                walletManagementMenu();
                break;
            case 3:
                changePasswordProcess();
                break;
            case 4:
                updateProfileProcess();
                break;
            case 9:
                authManager->logout(currentUsername);
                currentUsername.clear();
                std::cout << "Đã đăng xuất." << std::endl;
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

void System::showUserMenu() {
    int choice = 0;
    
    // Kiểm tra xem người dùng có ví chưa
    User* user = userManager->getUser(currentUsername);
    if (user && !user->hasWallet()) {
        std::cout << "\nBạn chưa có ví. Hệ thống sẽ tạo ví cho bạn." << std::endl;
        walletManager->createWallet(currentUsername);
        
        // Cập nhật ID ví cho người dùng
        Wallet* wallet = walletManager->getUserWallet(currentUsername);
        if (wallet) {
            user->setWalletId(wallet->getWalletId());
            userManager->saveData();
            std::cout << "Đã tạo ví với ID: " << wallet->getWalletId() << std::endl;
        }
    }
    
    // Kiểm tra mật khẩu tạm thời
    if (user && user->isPasswordTemporary()) {
        std::cout << "\nBạn đang sử dụng mật khẩu tạm thời. Vui lòng thay đổi mật khẩu ngay." << std::endl;
        changePasswordProcess();
    }
    
    do {
        int balance = transactionManager->getUserBalance(currentUsername);
        
        std::cout << "\n========== MENU NGƯỜI DÙNG ==========\n" << std::endl;
        std::cout << "Xin chào, " << user->getFullName() << "!" << std::endl;
        std::cout << "Số dư ví: " << balance << " điểm" << std::endl;
        std::cout << "\n1. Thông tin tài khoản" << std::endl;
        std::cout << "2. Quản lý ví" << std::endl;
        std::cout << "3. Thay đổi mật khẩu" << std::endl;
        std::cout << "4. Cập nhật thông tin cá nhân" << std::endl;
        std::cout << "9. Đăng xuất" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                userProfileMenu();
                break;
            case 2:
                walletManagementMenu();
                break;
            case 3:
                changePasswordProcess();
                break;
            case 4:
                updateProfileProcess();
                break;
            case 9:
                authManager->logout(currentUsername);
                currentUsername.clear();
                std::cout << "Đã đăng xuất." << std::endl;
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

void System::userProfileMenu() {
    User* user = userManager->getUser(currentUsername);
    if (!user) return;
    
    std::cout << "\n========== THÔNG TIN TÀI KHOẢN ==========\n" << std::endl;
    std::cout << "Tên đăng nhập: " << user->getUsername() << std::endl;
    std::cout << "Họ và tên: " << user->getFullName() << std::endl;
    std::cout << "Email: " << user->getEmail() << std::endl;
    std::cout << "Số điện thoại: " << user->getPhoneNumber() << std::endl;
    std::cout << "Địa chỉ: " << user->getAddress() << std::endl;
    std::cout << "Loại tài khoản: " << (user->getIsAdmin() ? "Quản trị viên" : "Người dùng thông thường") << std::endl;
    
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::cin.get();
}

void System::changePasswordProcess() {
    std::string oldPassword, newPassword, confirmPassword;
    
    std::cout << "\n========== THAY ĐỔI MẬT KHẨU ==========\n" << std::endl;
    
    std::cout << "Mật khẩu hiện tại: ";
    std::getline(std::cin, oldPassword);
    
    std::cout << "Mật khẩu mới: ";
    std::getline(std::cin, newPassword);
    
    std::cout << "Xác nhận mật khẩu mới: ";
    std::getline(std::cin, confirmPassword);
    
    if (newPassword != confirmPassword) {
        std::cout << "Mật khẩu mới không khớp!" << std::endl;
        return;
    }
    
    if (userManager->changeUserPassword(currentUsername, oldPassword, newPassword)) {
        std::cout << "Đã thay đổi mật khẩu thành công!" << std::endl;
    }
}

void System::updateProfileProcess() {
    User* user = userManager->getUser(currentUsername);
    if (!user) return;
    
    std::string newFullName, newEmail, newPhoneNumber, newAddress;
    
    std::cout << "\n========== CẬP NHẬT THÔNG TIN CÁ NHÂN ==========\n" << std::endl;
    std::cout << "(Để trống nếu không muốn thay đổi)" << std::endl;
    
    std::cout << "Họ và tên hiện tại: " << user->getFullName() << std::endl;
    std::cout << "Họ và tên mới: ";
    std::getline(std::cin, newFullName);
    
    std::cout << "Email hiện tại: " << user->getEmail() << std::endl;
    std::cout << "Email mới: ";
    std::getline(std::cin, newEmail);
    
    std::cout << "Số điện thoại hiện tại: " << user->getPhoneNumber() << std::endl;
    std::cout << "Số điện thoại mới: ";
    std::getline(std::cin, newPhoneNumber);
    
    std::cout << "Địa chỉ hiện tại: " << user->getAddress() << std::endl;
    std::cout << "Địa chỉ mới: ";
    std::getline(std::cin, newAddress);
    
    // Kiểm tra xem có thay đổi nào không
    if (newFullName.empty() && newEmail.empty() && newPhoneNumber.empty() && newAddress.empty()) {
        std::cout << "Không có thông tin nào được thay đổi." << std::endl;
        return;
    }
    
    if (userManager->requestUserInfoUpdate(currentUsername, newFullName, newEmail, newPhoneNumber, newAddress)) {
        std::string otp;
        std::cout << "\nNhập mã OTP đã được gửi để xác nhận thay đổi: ";
        std::getline(std::cin, otp);
        
        if (userManager->confirmUserInfoUpdate(currentUsername, otp)) {
            std::cout << "Thông tin cá nhân đã được cập nhật thành công!" << std::endl;
        } else {
            std::cout << "Xác thực OTP thất bại. Thông tin không được cập nhật." << std::endl;
        }
    }
}

void System::walletManagementMenu() {
    int choice = 0;
    
    do {
        User* user = userManager->getUser(currentUsername);
        if (!user) return;
        
        Wallet* wallet = walletManager->getUserWallet(currentUsername);
        if (!wallet) {
            std::cout << "Lỗi: Không tìm thấy ví của bạn!" << std::endl;
            return;
        }
        
        std::cout << "\n========== QUẢN LÝ VÍ ==========\n" << std::endl;
        std::cout << "ID ví: " << wallet->getWalletId() << std::endl;
        std::cout << "Số dư: " << wallet->getBalance() << " điểm" << std::endl;
        
        std::cout << "\n1. Chuyển điểm" << std::endl;
        std::cout << "2. Xem lịch sử giao dịch" << std::endl;
        
        // Thêm tùy chọn nạp điểm cho admin
        if (user->getIsAdmin()) {
            std::cout << "3. Nạp điểm cho người dùng (Admin)" << std::endl;
        }
        
        std::cout << "0. Quay lại" << std::endl;
        std::cout << "\nLựa chọn của bạn: ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                transferPoints();
                break;
            case 2:
                viewTransactionHistory();
                break;
            case 3:
                if (user->getIsAdmin()) {
                    addPointsToUserWallet();
                } else {
                    std::cout << "Lựa chọn không hợp lệ." << std::endl;
                }
                break;
            case 0:
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
                break;
        }
    } while (choice != 0);
}

void System::transferPoints() {
    std::string destinationWalletId;
    int amount;
    std::string description;
    
    std::cout << "\n========== CHUYỂN ĐIỂM ==========\n" << std::endl;
    
    std::cout << "Nhập ID ví đích: ";
    std::getline(std::cin, destinationWalletId);
    
    // Kiểm tra ví đích
    if (!walletManager->walletExists(destinationWalletId)) {
        std::cout << "Ví đích không tồn tại!" << std::endl;
        return;
    }
    
    // Kiểm tra không tự chuyển cho chính mình
    Wallet* sourceWallet = walletManager->getUserWallet(currentUsername);
    if (sourceWallet && sourceWallet->getWalletId() == destinationWalletId) {
        std::cout << "Không thể chuyển điểm cho chính mình!" << std::endl;
        return;
    }
    
    std::cout << "Nhập số điểm muốn chuyển: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (amount <= 0) {
        std::cout << "Số điểm không hợp lệ!" << std::endl;
        return;
    }
    
    std::cout << "Mô tả giao dịch (tùy chọn): ";
    std::getline(std::cin, description);
    
    // Khởi tạo giao dịch
    if (transactionManager->initiateTransaction(currentUsername, destinationWalletId, amount, description)) {
        std::string otp;
        std::cout << "\nNhập mã OTP đã được gửi để xác nhận giao dịch: ";
        std::getline(std::cin, otp);
        
        if (transactionManager->confirmTransaction(currentUsername, otp)) {
            std::cout << "Giao dịch thành công!" << std::endl;
        } else {
            std::cout << "Xác thực OTP thất bại. Giao dịch không được thực hiện." << std::endl;
        }
    }
}

void System::viewTransactionHistory() {
    auto transactions = transactionManager->getUserTransactionHistory(currentUsername);
    
    std::cout << "\n========== LỊCH SỬ GIAO DỊCH ==========\n" << std::endl;
    
    if (transactions.empty()) {
        std::cout << "Chưa có giao dịch nào." << std::endl;
        std::cout << "\nNhấn Enter để tiếp tục...";
        std::cin.get();
        return;
    }
    
    Wallet* userWallet = walletManager->getUserWallet(currentUsername);
    std::string userWalletId = userWallet ? userWallet->getWalletId() : "";
    
    std::cout << std::left << std::setw(15) << "Mã giao dịch"
              << std::setw(15) << "Loại"
              << std::setw(10) << "Số điểm"
              << std::setw(15) << "Trạng thái"
              << "Mô tả" << std::endl;
    
    std::cout << std::string(70, '-') << std::endl;
    
    for (const auto& transaction : transactions) {
        std::string type;
        std::string amount;
        
        if (transaction.getSourceWalletId() == userWalletId) {
            type = "Chuyển đi";
            amount = "- " + std::to_string(transaction.getAmount());
        } else {
            type = "Nhận về";
            amount = "+ " + std::to_string(transaction.getAmount());
        }
        
        std::cout << std::left << std::setw(15) << transaction.getTransactionId()
                  << std::setw(15) << type
                  << std::setw(10) << amount
                  << std::setw(15) << Transaction::statusToString(transaction.getStatus())
                  << transaction.getDescription() << std::endl;
    }
    
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::cin.get();
}

void System::userManagementMenu() {
    int choice = 0;
    
    do {
        std::cout << "\n========== QUẢN LÝ NGƯỜI DÙNG ==========\n" << std::endl;
        std::cout << "1. Tạo tài khoản mới" << std::endl;
        std::cout << "2. Xem danh sách người dùng" << std::endl;
        std::cout << "3. Tìm kiếm người dùng" << std::endl;
        std::cout << "4. Cập nhật thông tin người dùng" << std::endl;
        std::cout << "5. Nạp điểm cho người dùng" << std::endl;
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
            case 4:
                updateUserProfileByAdmin();
                break;
            case 5:
                addPointsToUserWallet();
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

void System::updateUserProfileByAdmin() {
    std::string targetUsername;
    
    std::cout << "\n========== CẬP NHẬT THÔNG TIN NGƯỜI DÙNG ==========\n" << std::endl;
    
    std::cout << "Nhập tên đăng nhập người dùng cần cập nhật: ";
    std::getline(std::cin, targetUsername);
    
    if (!userManager->userExists(targetUsername)) {
        std::cout << "Người dùng không tồn tại!" << std::endl;
        return;
    }
    
    User* targetUser = userManager->getUser(targetUsername);
    
    std::string newFullName, newEmail, newPhoneNumber, newAddress;
    
    std::cout << "\n(Để trống nếu không muốn thay đổi)" << std::endl;
    
    std::cout << "Họ và tên hiện tại: " << targetUser->getFullName() << std::endl;
    std::cout << "Họ và tên mới: ";
    std::getline(std::cin, newFullName);
    
    std::cout << "Email hiện tại: " << targetUser->getEmail() << std::endl;
    std::cout << "Email mới: ";
    std::getline(std::cin, newEmail);
    
    std::cout << "Số điện thoại hiện tại: " << targetUser->getPhoneNumber() << std::endl;
    std::cout << "Số điện thoại mới: ";
    std::getline(std::cin, newPhoneNumber);
    
    std::cout << "Địa chỉ hiện tại: " << targetUser->getAddress() << std::endl;
    std::cout << "Địa chỉ mới: ";
    std::getline(std::cin, newAddress);
    
    // Kiểm tra xem có thay đổi nào không
    if (newFullName.empty() && newEmail.empty() && newPhoneNumber.empty() && newAddress.empty()) {
        std::cout << "Không có thông tin nào được thay đổi." << std::endl;
        return;
    }
    
    if (userManager->requestUserInfoUpdateByAdmin(currentUsername, targetUsername, 
                                                newFullName, newEmail, 
                                                newPhoneNumber, newAddress)) {
        std::cout << "\nYêu cầu người dùng " << targetUsername << " xác nhận thay đổi qua OTP." << std::endl;
        std::cout << "Vui lòng nhập mã OTP từ người dùng đó: ";
        
        std::string otp;
        std::getline(std::cin, otp);
        
        if (userManager->confirmUserInfoUpdate(targetUsername, otp)) {
            std::cout << "Thông tin người dùng đã được cập nhật thành công!" << std::endl;
        } else {
            std::cout << "Xác thực OTP thất bại. Thông tin không được cập nhật." << std::endl;
        }
    }
}

void System::addPointsToUserWallet() {
    std::string targetUsername;
    int amount;
    std::string description;
    
    std::cout << "\n========== NẠP ĐIỂM CHO NGƯỜI DÙNG ==========\n" << std::endl;
    
    std::cout << "Nhập tên đăng nhập người dùng cần nạp điểm: ";
    std::getline(std::cin, targetUsername);
    
    if (!userManager->userExists(targetUsername)) {
        std::cout << "Người dùng không tồn tại!" << std::endl;
        return;
    }
    
    std::cout << "Nhập số điểm muốn nạp: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (amount <= 0) {
        std::cout << "Số điểm không hợp lệ!" << std::endl;
        return;
    }
    
    std::cout << "Mô tả (tùy chọn): ";
    std::getline(std::cin, description);
    
    if (transactionManager->addPointsFromMasterWallet(currentUsername, targetUsername, amount, description)) {
        std::cout << "Đã nạp " << amount << " điểm cho người dùng " << targetUsername << " thành công!" << std::endl;
    }
}
