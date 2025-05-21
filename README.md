## Intro
Dự án quản lý ví điểm thưởng

## Members
1. Bùi Trung Nghĩa - K24DTCN465 (Dựng base, làm tài liệu + Thiết kế, triển khai phần tạo, list user, admin, các chức năng người dùng)
2. Lê Chí Dũng - K24DTCN433 (Thiết kế design, triển khai phần OTP, Authentication)
3. Lê Bảo Hưng - K24DTCN445 (Thiết kế, triển khai phần ví, transaction, chuyển điểm giữa các ví)
4. ~~Nguyễn Quang Huy~~

## Summary
Hệ thống được xây dựng với kiến trúc hướng đối tượng, bao gồm các thành phần chính:

1. Quản lý người dùng: Đăng ký, đăng nhập, phân quyền, và quản lý thông tin cá nhân
2. Xác thực an toàn: Mã hóa mật khẩu, tạo và xác thực OTP
3. Quản lý ví điểm: Tạo ví, kiểm tra số dư, theo dõi lịch sử giao dịch
4. Giao dịch điểm thưởng: Chuyển điểm giữa các ví, xác thực giao dịch bằng OTP

Các bản đặc tả chức năng nằm ở thư mục `detail-design`

## Structure
```
├── README.md ## README
├── class-diagram.md ## Bản mô tả các class
├── detail-design ## Thư mục chứa thiết kế chi tiết
├── git-flow.drawio.png ## Quy trình sử dụng git
├── include ## Chứa header files
├── src ## Chứa source files
├── *.dat ## Database
```

## Installation
1. Prerequisites
    - g++ 
2. Download
   - Download from this repository 
3. Installation
    - Run `./run.sh`