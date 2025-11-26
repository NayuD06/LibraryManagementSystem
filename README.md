# Library Management System

Hệ thống quản lý thư viện C++ console với giao diện tiếng Anh, phân quyền 3 vai trò, và **auto-save** sau mỗi thao tác.

## Tính Năng Chính

- **Quản lý sách**: Thêm/sửa/xóa, tìm kiếm, theo dõi tồn kho
- **Quản lý user**: Đăng ký/đăng nhập, phân quyền, cập nhật hồ sơ
- **Mượn/trả sách**: Mượn 14 ngày, gia hạn, tính phí phạt tự động
- **Đặt trước sách**: Đặt trước sách đang được mượn (có thể sửa/xóa)
- **Báo cáo**: Thống kê tổng quan, top sách/user, sách quá hạn
- **Auto-save**: Tự động lưu data sau mỗi thao tác (mượn/trả/thêm/sửa/xóa)

## Phân Quyền

### User (Reader) - 4 chức năng
1. **Browse & Search** - Xem/tìm sách
   - View available books
   - Search by title
   - Search by author
   - View by category
   - View popular books (sorted by views)
   
2. **Manage Requests** - Quản lý đặt trước sách
   - Create a request (đặt trước sách)
   - View my requests (xem TẤT CẢ đơn đặt trước của tôi - bao gồm Pending, Confirmed, Fulfilled, Cancelled, Expired)
   - Edit request (sửa thông tin đơn: đổi sách, gia hạn ngày hết hạn - chỉ với trạng thái Pending/Confirmed)
   - Cancel request (hủy đơn đặt trước - chỉ với trạng thái Pending/Confirmed)
   
3. **My Profile** - Quản lý hồ sơ cá nhân
   - View profile
   - Update profile (name, email, phone)
   
4. **Change Password** - Đổi mật khẩu

**⚠️ Bảo mật:** User **không thể** tự mượn/trả/gia hạn sách. Tất cả thao tác mượn/trả sách chỉ có **Librarian** mới thực hiện được.

---

### Librarian - 6 chức năng (+ tất cả chức năng User)
1. **Manage Books** - Quản lý sách
   - Add new book
   - Update book
   - Remove book
   - View all books
   - Search books (by title/author/keyword)
   
2. **Manage Orders (Loans)** - Quản lý mượn/trả sách
   - Issue book to user (cho user mượn sách)
   - Process book return (nhận trả sách, tính phí phạt)
   - Renew loan for user (gia hạn sách cho user, tối đa 2 lần, không cho phép gia hạn sách quá hạn)
   - View all orders
   - View active loans
   - View overdue loans

3. **Manage Requests** - Quản lý đơn đặt trước
   - View all requests (xem tất cả đơn đặt trước của mọi user)
   - Confirm request (xác nhận đơn đặt trước)
   - Fulfill request (đáp ứng đơn - sách đã sẵn sàng cho user)
   - Cancel request (hủy đơn đặt trước)
   
4. **Generate Reports** - Tạo báo cáo
   - Borrowing report (báo cáo mượn/trả)
   - Inventory report (báo cáo tồn kho)
   
5. **Access User Features** - Truy cập tất cả chức năng User

6. **My Profile** - Xem hồ sơ cá nhân

**🔑 Quyền đặc biệt:** Chỉ Librarian mới có quyền cho user mượn sách, nhận trả sách, gia hạn sách cho user, và xử lý các đơn đặt trước (confirm/fulfill/cancel).

---

### Admin - 5 chức năng (+ tất cả chức năng Librarian & User)
1. **User Management** - Quản lý người dùng
   - Create Librarian account (tạo tài khoản thủ thư)
   - View all users
   - View users by role (Admin/Librarian/User)
   - Deactivate user (vô hiệu hóa tài khoản)
   - Activate user (kích hoạt lại tài khoản)
   
2. **System Reports** - Báo cáo hệ thống
   - General statistics (thống kê tổng quan)
   - User list (danh sách người dùng)
   - Most borrowed books (top sách được mượn nhiều nhất)
   - Most active users (top người dùng tích cực nhất)
   - Popular categories (thể loại phổ biến)
   - Overdue books report (báo cáo sách quá hạn)
   
3. **Data Management** - Quản lý dữ liệu
   - Backup data (sao lưu dữ liệu)
   - Restore data (khôi phục dữ liệu)
   
4. **Access Librarian Features** - Truy cập tất cả chức năng Librarian

5. **Access User Features** - Truy cập tất cả chức năng User

6. **My Profile** - Xem hồ sơ cá nhân

**👑 Quyền tối cao:** Admin có toàn quyền quản lý hệ thống, tạo tài khoản Librarian, quản lý người dùng, và truy cập mọi chức năng.

## Cấu Trúc Project

```
LibraryManagementSystem/
├── main.cpp                      # Ứng dụng chính (1300+ dòng)
├── LibraryManagement.exe         # File thực thi
│
├── Data Files (auto-saved):
│   ├── users.txt                 # Dữ liệu người dùng
│   ├── books.txt                 # Dữ liệu sách
│   ├── orders.txt                # Đơn mượn sách
│   └── requests.txt              # Đặt trước sách
│
├── UserManagement/
│   ├── User.h/cpp               # Model User (6 fields, XOR encryption)
│   └── Services/
│       └── UserService.h/cpp    # Quản lý user, save/load
│
├── BookManagement/
│   ├── Book.h/cpp               # Model Book (13 fields, no purchasePrice)
│   └── Library.h/cpp            # Quản lý thư viện, save/load
│
├── BorrowingProcess/Models/
│   ├── Order.h/cpp              # Đơn mượn, tính phí phạt
│   └── Request.h/cpp            # Đặt trước sách
│
└── AdminAndReporting/Reports/
    └── ReportGenerator.h/cpp    # Báo cáo thống kê
```

## Biên Dịch & Chạy (Windows)

### Cách 1: Build & Run Trực Tiếp (KHUYẾN NGHỊ)

```bash
# Compile
g++ -std=c++11 -o LibraryManagement.exe main.cpp UserManagement/User.cpp UserManagement/Services/UserService.cpp BookManagement/Book.cpp BookManagement/Library.cpp BorrowingProcess/Models/Order.cpp BorrowingProcess/Models/Request.cpp AdminAndReporting/Reports/ReportGenerator.cpp

# Run
.\LibraryManagement.exe
```

### Cách 2: Chạy File .exe Có Sẵn

Nếu đã có file `LibraryManagement.exe` build sẵn:
```bash
.\LibraryManagement.exe
```

### Cách 3: Docker (⚠️ KHÔNG KHUYẾN NGHỊ)

**⚠️ LƯU Ý:** Console app **KHÔNG CHẠY ỔN ĐỊNH** trong Docker:
- ❌ Input/output bị lỗi (menu lặp vô hạn, không nhận input)
- ❌ Dữ liệu có thể bị mất khi container bị xóa
- ✅ **Khuyến nghị:** Chạy trực tiếp file `.exe` trên Windows

Nếu cần Docker để nộp bài:
```bash
docker build -t library-management .
docker run -it --rm library-management
```

## Tài Khoản Mặc Định

Hệ thống tự tạo 3 tài khoản mẫu khi chạy lần đầu:

| Vai trò | Email | Password |
|---------|-------|----------|
| Admin | admin@library.com | admin123 |
| Librarian | librarian1@library.com | lib123 |
| User | reader1@library.com | read123 |

## Hướng Dẫn Sử Dụng

### User
1. Đăng nhập: `reader1@library.com` / `read123`
2. Tìm sách → Xem đơn mượn hiện tại
3. Đặt trước sách, yêu cầu sách mới
4. Cập nhật profile, đổi mật khẩu

**Lưu ý:** User không thể tự mượn/trả/gia hạn sách, tất cả phải qua Librarian

### Librarian
1. Đăng nhập: `librarian1@library.com` / `lib123`
2. Thêm sách mới → Cho user mượn (Issue book)
3. Nhận trả sách (Process return) → Tính phí nếu trễ/hư hỏng
4. Gia hạn sách cho user (Renew loan) - tối đa 2 lần, không cho phép gia hạn sách quá hạn
5. Duyệt yêu cầu đặt sách mới
6. Xem báo cáo mượn/trả, tồn kho

### Admin
1. Đăng nhập: `admin@library.com` / `admin123`
2. Tạo tài khoản Librarian
3. Xem thống kê hệ thống
4. Backup/restore dữ liệu

## Đặc Điểm Kỹ Thuật

- **Ngôn ngữ:** C++
- **Giao diện:** Console (tiếng Anh)
- **Comment:** Tiếng Việt
- **Kiến trúc:** OOP, Service Layer Pattern
- **Lưu trữ:** 4 file text (users, books, orders, requests)
- **Bảo mật:** 
  - XOR password encryption
  - User không thể tự mượn/trả/gia hạn sách (chỉ Librarian)
  - Giới hạn gia hạn tối đa 2 lần/đơn
  - Không cho phép gia hạn sách quá hạn
- **Phân quyền:** User < Librarian < Admin
- **Auto-save:** Tự động lưu sau mỗi thao tác (create/update/delete)
- **Data persistence:** Load tự động khi khởi động, không mất data khi tắt
- **Validation:** Safe string parsing, empty line handling, status auto-update

## Chức Năng Auto-Save

Hệ thống **tự động lưu** dữ liệu ngay sau các thao tác:
- ✅ Đăng ký user mới
- ✅ Thêm/sửa/xóa sách
- ✅ Mượn/trả/gia hạn sách (Librarian issue book, process return, renew loan)
- ✅ Đặt trước sách (reserve)
- ✅ Cập nhật profile/password
- ✅ Kích hoạt/vô hiệu user

→ **Không cần save thủ công**, data được persist tự động vào 4 file .txt

## Known Issues

- **Duplicate requests**: Hệ thống hiện cho phép user đặt trước cùng 1 cuốn sách nhiều lần 
- **Book status**: Sách với quantity=0 tự động chuyển sang trạng thái "Borrowed"
- **Docker incompatibility**: Console app không chạy ổn định trong Docker (input/output errors, infinite menu loop)

## Deployment

- **Khuyến nghị:** Build và chạy trực tiếp trên Windows bằng g++
- **Để chia sẻ:** Gửi toàn bộ source code + file `LibraryManagement.exe`
- **Docker:** Có sẵn Dockerfile nhưng **không khuyến nghị** chạy thực tế vì bị lỗi console

---

**Version:** 2.6 (Renamed Reservation to Request + added Edit/Delete request functionality)  
**Updated:** 27/11/2025
