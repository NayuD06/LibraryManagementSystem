# Library Management System

Hệ thống quản lý thư viện C++ console với giao diện tiếng Anh, phân quyền 3 vai trò, và **auto-save** sau mỗi thao tác.

## Tính Năng Chính

- **Quản lý sách**: Thêm/sửa/xóa, tìm kiếm, theo dõi tồn kho
- **Quản lý user**: Đăng ký/đăng nhập, phân quyền, cập nhật hồ sơ
- **Mượn/trả sách**: Mượn 14 ngày, gia hạn, tính phí phạt tự động
- **Đặt trước & yêu cầu**: Đặt trước sách, yêu cầu sách mới, duyệt/từ chối
- **Báo cáo**: Thống kê tổng quan, top sách/user, sách quá hạn
- **Auto-save**: Tự động lưu data sau mỗi thao tác (mượn/trả/thêm/sửa/xóa)

## Phân Quyền

### User (8 chức năng)
1. **Browse & Search** - Xem/tìm sách (5 options)
2. **Borrow a book** - Mượn sách
3. **Return a book** - Trả sách
4. **Renew a loan** - Gia hạn
5. **Manage reservations** - Đặt trước (2 options)
6. **Manage book requests** - Yêu cầu sách mới (3 options)
7. **My profile** - Quản lý hồ sơ (2 options)
8. **Change password** - Đổi mật khẩu

### Librarian (6 chức năng + tất cả chức năng User)
1. **Manage Books** - Quản lý sách (5 options: add, update, remove, view, search)
2. **Manage Loans** - Quản lý mượn/trả (4 options: issue, return, view active, view overdue)
3. **Manage Requests** - Quản lý yêu cầu (3 options: view, approve, reject)
4. **Generate Reports** - Báo cáo (2 options: borrowing, inventory)
5. **Access User Features** - Truy cập chức năng User
6. **My Profile** - Xem hồ sơ

### Admin (6 chức năng + tất cả chức năng Librarian & User)
1. **User Management** - Quản lý user (5 options: create librarian, view all, view by role, deactivate, activate)
2. **System Reports** - Báo cáo hệ thống (6 options: general stats, user list, most borrowed, most active, popular categories, overdue)
3. **Data Management** - Quản lý dữ liệu (2 options: backup, restore)
4. **Access Librarian Features** - Truy cập chức năng Librarian
5. **Access User Features** - Truy cập chức năng User
6. **My Profile** - Xem hồ sơ

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
│   ├── reservations.txt          # Đặt trước
│   └── bookrequests.txt          # Yêu cầu sách mới
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
│   ├── Reservation.h/cpp        # Đặt trước sách
│   └── BookRequest.h/cpp        # Yêu cầu sách mới
│
└── AdminAndReporting/Reports/
    └── ReportGenerator.h/cpp    # Báo cáo thống kê
```

## Biên Dịch

**Windows:**
```bash
g++ -std=c++11 -o LibraryManagement.exe main.cpp UserManagement/User.cpp UserManagement/Services/UserService.cpp BookManagement/Book.cpp BookManagement/Library.cpp BorrowingProcess/Models/Order.cpp BorrowingProcess/Models/Reservation.cpp BorrowingProcess/Models/BookRequest.cpp AdminAndReporting/Reports/ReportGenerator.cpp
```

**Linux/macOS:**
```bash
g++ -std=c++11 -o LibraryManagement main.cpp UserManagement/User.cpp UserManagement/Services/UserService.cpp BookManagement/Book.cpp BookManagement/Library.cpp BorrowingProcess/Models/Order.cpp BorrowingProcess/Models/Reservation.cpp BorrowingProcess/Models/BookRequest.cpp AdminAndReporting/Reports/ReportGenerator.cpp
```

## Chạy Chương Trình

**Windows:** `.\LibraryManagement.exe`  
**Linux/macOS:** `./LibraryManagement`

## Tài Khoản Mặc Định

Hệ thống tự tạo 6 tài khoản mẫu khi chạy lần đầu:

| Vai trò | Email | Password |
|---------|-------|----------|
| Admin | admin@library.com | admin123 |
| Librarian | librarian1@library.com | lib123 |
| Librarian | librarian2@library.com | lib123 |
| User | reader1@library.com | read123 |
| User | reader2@library.com | read123 |
| User | reader3@library.com | read123 |

## Hướng Dẫn Sử Dụng

### User
1. Đăng nhập: `reader1@library.com` / `read123`
2. Tìm sách → Mượn → Xem đơn → Trả
3. Đặt trước sách đang mượn
4. Yêu cầu sách mới

### Librarian
1. Đăng nhập: `librarian1@library.com` / `lib123`
2. Thêm sách mới → Cho mượn
3. Nhận trả sách → Duyệt yêu cầu
4. Xem báo cáo mượn/trả, tồn kho

### Admin
1. Đăng nhập: `admin@library.com` / `admin123`
2. Tạo tài khoản Librarian
3. Xem thống kê hệ thống
4. Backup/restore dữ liệu

## Đặc Điểm Kỹ Thuật

- **Ngôn ngữ:** C++11
- **Giao diện:** Console (tiếng Anh)
- **Comment:** Tiếng Việt
- **Kiến trúc:** OOP, Service Layer Pattern
- **Lưu trữ:** 5 file text (users, books, orders, reservations, bookrequests)
- **Bảo mật:** XOR password encryption
- **Phân quyền:** User < Librarian < Admin
- **Auto-save:** Tự động lưu sau mỗi thao tác (create/update/delete)
- **Data persistence:** Load tự động khi khởi động, không mất data khi tắt

## Chức Năng Auto-Save

Hệ thống **tự động lưu** dữ liệu ngay sau các thao tác:
- ✅ Đăng ký user mới
- ✅ Thêm/sửa/xóa sách
- ✅ Mượn/trả/gia hạn sách
- ✅ Đặt trước/yêu cầu sách
- ✅ Duyệt/từ chối yêu cầu
- ✅ Cập nhật profile/password
- ✅ Kích hoạt/vô hiệu user

→ **Không cần save thủ công**, data được persist tự động vào 5 file .txt

---

**Version:** 2.1 (Auto-save enabled)  
**Updated:** 19/11/2025
