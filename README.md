# Hệ Thống Quản Lý Thư Viện

Hệ thống quản lý thư viện console bằng C++ với giao diện tiếng Anh và comment tiếng Việt. Hỗ trợ đầy đủ chức năng quản lý sách, người dùng, mượn/trả, đặt trước và báo cáo với phân quyền theo vai trò.

## Tổng Quan

Hệ thống thiết kế theo kiến trúc phân quyền phân cấp (Độc giả → Thủ thư → Quản trị viên) với giao diện tiếng Anh và code comment tiếng Việt.

## Tính Năng Chính

### 1. Phân Quyền Theo Vai Trò (Kế Thừa Phân Cấp)
- **Độc giả**: 18 chức năng cơ bản (xem, tìm, mượn, đặt trước, yêu cầu, đánh giá, quản lý hồ sơ)
- **Thủ thư**: Kế thừa Độc giả + 16 chức năng quản lý (thêm/sửa/xóa sách, cho mượn/thu hồi, duyệt yêu cầu, báo cáo)
- **Quản trị**: Kế thừa Thủ thư + Độc giả + 16 chức năng admin (tạo tài khoản, quản lý user, báo cáo hệ thống, backup)

### 2. Quản Lý Sách
- Thêm/sửa/xóa sách với đầy đủ thông tin (tên, tác giả, NXB, năm, ISBN, danh mục, từ khóa, số lượng, trang, mô tả, giá)
- Tìm kiếm theo tên/tác giả/từ khóa
- Xem theo danh mục, sắp xếp theo độ phổ biến
- Theo dõi tồn kho realtime

### 3. Quản Lý Người Dùng (Admin)
- Tạo tài khoản Thủ thư
- Xem tất cả user, lọc theo vai trò
- Kích hoạt/vô hiệu hóa tài khoản
- User tự cập nhật hồ sơ và đổi mật khẩu

### 4. Hệ Thống Mượn/Trả
- Mượn sách (14 ngày), gia hạn, trả sách
- Xem lịch sử mượn
- Thủ thư cho mượn/nhận trả cho bất kỳ user nào
- Cập nhật tồn kho tự động

### 5. Đặt Trước & Yêu Cầu Sách
- Đặt trước sách đang mượn (hết hạn sau 7 ngày)
- Yêu cầu sách mới chưa có trong thư viện
- Xem/hủy đặt trước và yêu cầu
- Thủ thư duyệt/từ chối yêu cầu

### 6. Đánh Giá & Review
- Đánh giá 1-5 sao và viết review cho sách đã trả

### 7. Báo Cáo (Thủ thư & Admin)
- Xu hướng mượn, tồn kho, thống kê tổng quan
- Top 10 sách/user phổ biến nhất
- Danh mục phổ biến, sách quá hạn

### 8. Tìm Kiếm & Khám Phá
- Tìm theo tên/tác giả/từ khóa/danh mục
- Sắp xếp theo độ phổ biến (lượt xem)
- Lọc sách còn sẵn

### 9. Lưu Trữ & Backup
- Tự động lưu/load dữ liệu từ file
- Tạo tài khoản mẫu khi chạy lần đầu
- Backup/restore thủ công

## Phân Quyền Theo Vai Trò

### Độc Giả (18 Chức Năng)
1-5. Xem sách có sẵn, tìm theo tên/tác giả, xem theo danh mục, xem sách phổ biến
6-9. Mượn sách, xem lịch sử, gia hạn, trả sách
10-14. Đặt trước, xem đặt trước, yêu cầu sách mới, xem yêu cầu, hủy yêu cầu
15-18. Đánh giá sách, xem hồ sơ, cập nhật hồ sơ, đổi mật khẩu

### Thủ Thư (Tất cả chức năng Độc giả + 16 Chức Năng Quản Lý)
**Quản lý sách:** Thêm/sửa/xóa sách, xem tất cả, tìm kiếm nâng cao
**Quản lý mượn/trả:** Cho mượn, nhận trả, xem đơn đang mượn, xem quá hạn
**Quản lý yêu cầu:** Xem yêu cầu chờ, duyệt/từ chối yêu cầu
**Báo cáo:** Báo cáo mượn/trả, báo cáo tồn kho
**Cá nhân:** Xem hồ sơ, truy cập tất cả chức năng Độc giả

### Quản Trị Viên (Tất cả chức năng Thủ thư + Độc giả + 16 Chức Năng Admin)
**Quản lý user:** Tạo tài khoản Thủ thư, xem tất cả user, xem theo vai trò, kích hoạt/vô hiệu hóa
**Báo cáo hệ thống:** Thống kê tổng quan, danh sách user, top 10 sách/user phổ biến, danh mục phổ biến, sách quá hạn
**Quản lý dữ liệu:** Backup, restore hệ thống
**Cá nhân:** Xem hồ sơ, truy cập tất cả chức năng Thủ thư và Độc giả

## Project Structure

```
LibraryManagementSystem/
├── main.cpp                          # Main application (1,108 lines)
│                                     # - Full English interface
│                                     # - Vietnamese comments
│                                     # - Role-based menus (Reader/Librarian/Admin)
│                                     # - Global data: orders, reservations, bookRequests
│
├── LibraryManagement.exe             # Compiled executable (Windows, 1.1 MB)
├── users.txt                         # User data persistence file
├── books.txt                         # Book data persistence file
│
├── UserManagement/
│   ├── User.h                        # User model definition
│   │                                 # - Properties: id, fullName, DOB, gender, address, phone, email, password, role
│   │                                 # - Enums: Role (Reader/Librarian/Admin), Gender (Male/Female/Other)
│   │                                 # - Methods: getId(), getFullName(), displayUserInfo(), checkPassword()
│   │
│   ├── User.cpp                      # User model implementation
│   │                                 # - Password encryption (XOR-based)
│   │                                 # - Activity logging
│   │                                 # - Serialization/deserialization for file storage
│   │
│   └── Services/
│       ├── UserService.h             # User management service
│       │                             # - Methods: registerUser(), login(), logout(), findUserById(), findUserByEmail()
│       │                             # - Methods: updateUserProfile(), changePassword(), deactivateUser(), activateUser()
│       │                             # - Methods: getAllUsers(), getUsersByRole(), saveToFile(), loadFromFile()
│       │
│       └── UserService.cpp           # Service implementation
│                                     # - User authentication and authorization
│                                     # - User CRUD operations
│                                     # - File I/O for persistence
│
├── BookManagement/
│   ├── Book.h                        # Book model definition
│   │                                 # - Properties: bookId, title, author, publisher, year, ISBN
│   │                                 # - Properties: categories, keywords, totalQuantity, availableQuantity
│   │                                 # - Properties: pages, descriptions, status, condition, viewCount, prices
│   │                                 # - Methods: displayBookInfo(), isAvailable(), decreaseAvailableQuantity()
│   │
│   ├── Book.cpp                      # Book model implementation
│   │                                 # - Book information display
│   │                                 # - Quantity management
│   │                                 # - Serialization for storage
│   │
│   ├── Library.h                     # Library management service
│   │                                 # - Methods: addBook(), addNewBook(), updateBook(), removeBook()
│   │                                 # - Methods: findBookById(), findBookByISBN()
│   │                                 # - Methods: searchByTitle(), searchByAuthor(), searchByKeyword()
│   │                                 # - Methods: filterByCategory(), sortByPopularity()
│   │                                 # - Methods: displayAllBooks(), saveToFile(), loadFromFile()
│   │
│   └── Library.cpp                   # Library service implementation
│                                     # - Book inventory management
│                                     # - Search and filtering algorithms
│                                     # - File persistence
│
├── BorrowingProcess/
│   └── Models/
│       ├── Order.h                   # Borrowing order model
│       │                             # - Properties: orderId, userId, bookId, status, dates, rating, review
│       │                             # - Status: PENDING, ISSUED, RETURNED, OVERDUE, RESERVED
│       │                             # - Methods: createOrder(), updateStatus(), addReview(), renewOrder()
│       │
│       ├── Order.cpp                 # Order implementation
│       │                             # - Order lifecycle management
│       │                             # - Review and rating system
│       │
│       ├── Reservation.h             # Book reservation model
│       │                             # - Properties: reservationId, userId, bookId, dates, status, queuePosition
│       │                             # - Status: Pending, Confirmed, Cancelled, Expired, Fulfilled
│       │                             # - Methods: confirmReservation(), cancelReservation(), fulfill()
│       │
│       ├── Reservation.cpp           # Reservation implementation
│       │
│       ├── BookRequest.h             # Book request model
│       │                             # - Properties: requestId, userId, bookTitle, author, ISBN, dates, status
│       │                             # - Status: Pending, Approved, Rejected, Fulfilled, Cancelled
│       │                             # - Methods: approveRequest(), rejectRequest(), cancelRequest()
│       │
│       └── BookRequest.cpp           # Request implementation
│
└── AdminAndReporting/
    └── Reports/
        ├── ReportGenerator.h         # Static report generation class
        │                             # - Methods: generalStats(), inventory(), userList()
        │                             # - Methods: borrowingTrends(), popularCategories()
        │                             # - Methods: mostBorrowedBooks(), currentlyBorrowedBooks()
        │                             # - Methods: returningStatistics(), overdueBooks()
        │                             # - Methods: mostActiveUsers(), userActivityReport()
        │
        └── ReportGenerator.cpp       # Report generation implementation
                                      # - Full English output
                                      # - Statistical analysis
                                      # - Formatted console reports
```

## Mô Hình Dữ Liệu

**User:** ID, tên, DOB, giới tính, địa chỉ, SĐT, email, password (mã hóa), vai trò, lịch sử hoạt động, trạng thái, ngày đăng ký

**Book:** ID, tên, tác giả, NXB, năm XB, ISBN, danh mục, keywords, số lượng tổng/còn, trang, mô tả ngắn/dài, trạng thái, tình trạng, lượt xem, giá thuê/mua

**Order:** ID đơn, ID user, ID sách, ngày mượn/cho mượn/hạn trả/trả, trạng thái, tình trạng sách, số lần gia hạn, đánh giá, review

**Reservation:** ID đặt, ID user, ID sách, ngày đặt, ngày hết hạn, trạng thái, vị trí hàng đợi

**BookRequest:** ID yêu cầu, ID user, thông tin sách (tên, tác giả, ISBN), ngày yêu cầu, trạng thái, ngày duyệt, ghi chú admin

## Biên Dịch và Chạy Chương Trình

### Yêu Cầu
- Trình biên dịch C++ hỗ trợ C++11+ (g++, MSVC, clang)
- Windows, Linux, hoặc macOS

### Biên Dịch

**Windows (MinGW/g++):**
```bash
g++ -std=c++11 -o LibraryManagement.exe main.cpp UserManagement/User.cpp UserManagement/Services/UserService.cpp BookManagement/Book.cpp BookManagement/Library.cpp BorrowingProcess/Models/Order.cpp BorrowingProcess/Models/Reservation.cpp BorrowingProcess/Models/BookRequest.cpp AdminAndReporting/Reports/ReportGenerator.cpp
```

**Linux/macOS:**
```bash
g++ -std=c++11 -o LibraryManagement main.cpp UserManagement/User.cpp UserManagement/Services/UserService.cpp BookManagement/Book.cpp BookManagement/Library.cpp BorrowingProcess/Models/Order.cpp BorrowingProcess/Models/Reservation.cpp BorrowingProcess/Models/BookRequest.cpp AdminAndReporting/Reports/ReportGenerator.cpp
```

### Chạy Chương Trình

**Windows:** `LibraryManagement.exe`  
**Linux/macOS:** `./LibraryManagement`

## Tài Khoản Mặc Định

Hệ thống tự động tạo 6 tài khoản mẫu khi chạy lần đầu (nếu file `users.txt` không tồn tại hoặc rỗng):

**Quản trị viên (1 tài khoản):**
- Email: `admin@library.com` | Password: `admin123`

**Thủ thư (2 tài khoản):**
- Email: `librarian1@library.com` | Password: `lib123`
- Email: `librarian2@library.com` | Password: `lib123`

**Độc giả (3 tài khoản):**
- Email: `reader1@library.com` | Password: `read123`
- Email: `reader2@library.com` | Password: `read123`
- Email: `reader3@library.com` | Password: `read123`

## Hướng Dẫn Sử Dụng

### Khởi Động Lần Đầu
1. Biên dịch chương trình (xem mục Biên Dịch)
2. Chạy `LibraryManagement.exe`
3. Hệ thống tự tạo 6 tài khoản mẫu
4. Đăng nhập với tài khoản mặc định

### Độc Giả (18 Chức Năng)

**Đăng nhập:** `reader1@library.com` / `read123`

**Xem & Tìm sách:** Option 1-5 (xem sách có sẵn, tìm theo tên/tác giả, xem theo danh mục, xem phổ biến)

**Mượn/Trả:** 
- Option 6: Mượn sách → Nhập Book ID → Tự động tạo đơn (14 ngày)
- Option 7: Xem lịch sử mượn
- Option 8: Gia hạn → Nhập Order ID
- Option 9: Trả sách → Nhập Order ID

**Đặt trước & Yêu cầu:**
- Option 10: Đặt trước → Nhập Book ID (hết hạn sau 7 ngày)
- Option 11: Xem đặt trước
- Option 12: Yêu cầu sách mới → Nhập tên, tác giả, ISBN
- Option 13-14: Xem/hủy yêu cầu

**Đánh giá & Hồ sơ:**
- Option 15: Đánh giá sách → Nhập Order ID → Chọn 1-5 sao + viết review
- Option 16-18: Xem hồ sơ, cập nhật hồ sơ, đổi mật khẩu

### Thủ Thư (Tất cả chức năng Độc giả + 16 Quản Lý)

**Đăng nhập:** `librarian1@library.com` / `lib123`

**Quản lý sách:** Option 1-5 (thêm/sửa/xóa sách, xem tất cả, tìm kiếm)

**Quản lý mượn/trả:** Option 6-9 (cho mượn user, nhận trả, xem đơn đang mượn/quá hạn)

**Quản lý yêu cầu:** Option 10-12 (xem yêu cầu chờ, duyệt/từ chối)

**Báo cáo:** Option 13-14 (báo cáo mượn/trả, tồn kho)

**Truy cập Độc giả:** Option 16

### Quản Trị Viên (Tất cả chức năng Thủ thư + Độc giả + 16 Admin)

**Đăng nhập:** `admin@library.com` / `admin123`

**Quản lý user:** Option 1-5 (tạo Thủ thư, xem user, xem theo role, kích hoạt/vô hiệu hóa)

**Báo cáo hệ thống:** Option 6-11 (thống kê tổng quan, danh sách user, top 10 sách/user, danh mục phổ biến, quá hạn)

**Backup/Restore:** Option 12-13 (nhập tên file → lưu/phục hồi)

**Truy cập Thủ thư/Độc giả:** Option 15-16

### Quy Trình Mẫu

**1. Mượn và trả sách (Độc giả):**
```
Login → Tìm sách (Option 2) → Mượn (Option 6 + Book ID) → 
Xem lịch sử (Option 7) → Trả (Option 9 + Order ID) → Đánh giá (Option 15)
```

**2. Thêm sách và cho mượn (Thủ thư):**
```
Login → Thêm sách (Option 1) → Ghi nhớ Book ID → 
Cho mượn (Option 6 + email user + Book ID)
```

**3. Tạo user và backup (Admin):**
```
Login → Tạo Thủ thư (Option 1) → Xem user (Option 2) → 
Thống kê (Option 6) → Backup (Option 12 + tên file)
```

## Chi Tiết Kỹ Thuật

### Cấu Trúc main.cpp (1,108 dòng)
- Dòng 1-34: Headers, includes, khai báo dữ liệu global
- Dòng 36-62: Helper functions (prompt, toString, date)
- Dòng 64-429: Menu Độc giả (18 chức năng)
- Dòng 431-763: Menu Thủ thư (kế thừa Độc giả + 16 quản lý)
- Dòng 765-987: Menu Admin (kế thừa Thủ thư + Độc giả + 16 admin)
- Dòng 989-1108: Hàm main (khởi tạo, login/register, routing)

### Đặc Điểm Code
- **Giao diện**: 100% tiếng Anh
- **Comment**: 100% tiếng Việt (`// Xem sách có sẵn`, `// Tìm sách theo tên`)
- **Kiến trúc**: OOP, Service Layer, phân tách Models/Services/Reports
- **Phân quyền**: Kế thừa phân cấp (Admin → Thủ thư → Độc giả)
- **Dữ liệu**: Global vectors (orders, reservations, bookRequests), auto-increment IDs
- **File**: Auto-save khi thoát, auto-load khi khởi động

### Bảo Mật
- Mã hóa password (XOR) - nên dùng bcrypt cho production
- Phân quyền theo vai trò (Reader < Librarian < Admin)
- Logout để đổi user
- Admin kích hoạt/vô hiệu hóa tài khoản

## Xử Lý Lỗi Thường Gặp

**Lỗi "User not found" khi đăng nhập:** Xóa file `users.txt` và chạy lại để tạo tài khoản mẫu

**Lỗi biên dịch:** Kiểm tra đủ file .cpp/.h, dùng đúng lệnh biên dịch, kiểm tra C++11

**Cảnh báo "Could not open file":** Bình thường khi chạy lần đầu, file sẽ tự tạo khi thoát

## Nâng Cấp Tương Lai

1. Database (SQLite/MySQL) thay vì file
2. GUI (Qt/wxWidgets)
3. Tính phí phạt tự động
4. Gửi email nhắc hạn trả
5. Tìm kiếm nâng cao, export PDF/Excel
6. Web API, mobile app

## Thông Số Kỹ Thuật

- **Ngôn ngữ**: C++ (C++11)
- **Số dòng**: ~1,108 (main.cpp) + ~2,500 (tất cả file)
- **Kích thước**: 1.1 MB (Windows .exe)
- **Kiến trúc**: OOP, Service Layer, phân quyền kế thừa
- **Lưu trữ**: File serialize (users.txt, books.txt) + vectors trong RAM
- **Compiler**: GCC/MinGW, MSVC, Clang

---

**Phiên bản**: 1.0  
**Cập nhật**: 19/11/2025  
**Nhóm phát triển**: Library Management System Team
