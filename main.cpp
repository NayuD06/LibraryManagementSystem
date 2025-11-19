// ===== LIBRARY MANAGEMENT SYSTEM - MAIN FILE =====
// File chính điều khiển toàn bộ ứng dụng quản lý thư viện
// Chức năng:
// - Hiển thị menu chính (Login/Register/Exit)
// - Điều hướng đến menu theo vai trò (Reader/Librarian/Admin)
// - Quản lý Library, User, Order, Reservation, BookRequest

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "UserManagement/User.h"
#include "UserManagement/Services/UserService.h"
#include "BookManagement/Library.h"
#include "BookManagement/Book.h"
#include "BorrowingProcess/Models/Order.h"
#include "BorrowingProcess/Models/Reservation.h"
#include "BorrowingProcess/Models/BookRequest.h"
#include "AdminAndReporting/Reports/ReportGenerator.h"

using namespace std;

// ===== GLOBAL DATA =====
// Các container lưu trữ dữ liệu trong bộ nhớ
vector<Order> orders;              // Danh sách đơn mượn
vector<Reservation> reservations;  // Danh sách đặt trước
vector<BookRequest> bookRequests;  // Danh sách yêu cầu sách
int nextOrderId = 1;               // ID tiếp theo cho Order
int nextReservationId = 1;         // ID tiếp theo cho Reservation
int nextRequestId = 1;             // ID tiếp theo cho BookRequest

// ===== HELPER FUNCTIONS =====
// Hàm tiện ích chung

// Hàm nhận input từ người dùng
string prompt(const string &message) {
    cout << message;
    string value;
    getline(cin, value);
    return value;
}

// Hàm chuyển int thành string
string toString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// Hàm lấy ngày hiện tại (simplified)
string getCurrentDate() {
    return "2025-11-19"; // Placeholder - có thể dùng <ctime> để lấy ngày thực
}

// Hàm tính ngày đến hạn (thêm số ngày vào ngày hiện tại)
string calculateDueDate(int days) {
    return "2025-12-03"; // Placeholder - cộng days vào getCurrentDate()
}

// ===== READER MENU =====
// Chức năng dành cho người đọc (Reader):
// - Xem sách có sẵn, tìm kiếm sách
// - Mượn sách, xem lịch sử mượn
// - Đặt trước sách, yêu cầu sách mới
// - Gia hạn sách, đánh giá sách
// - Xem thông tin cá nhân
void readerMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== READER MENU (" << currentUser->getFullName() << ") ==========\n";
        cout << "1. View available books\n";
        cout << "2. Search books by title\n";
        cout << "3. Search books by author\n";
        cout << "4. View books by category\n";
        cout << "5. View popular books\n";
        cout << "6. Borrow a book\n";
        cout << "7. View my borrow history\n";
        cout << "8. Renew a loan\n";
        cout << "9. Return a book\n";
        cout << "10. Reserve a book\n";
        cout << "11. View my reservations\n";
        cout << "12. Request a new book\n";
        cout << "13. View my book requests\n";
        cout << "14. Cancel a request\n";
        cout << "15. Rate and review a book\n";
        cout << "16. View my profile\n";
        cout << "17. Update my profile\n";
        cout << "18. Change password\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Xem sách có sẵn
            cout << "\n--- Available Books ---\n";
            bool found = false;
            for (const auto &book : library.getAllBooks()) {
                if (book.getAvailableQuantity() > 0) {
                    book.displayBookInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            if (!found) cout << "No books available.\n";
            break;
        }
        
        case 2: { // Tìm sách theo tên
            string keyword = prompt("Enter book title: ");
            auto results = library.searchByTitle(keyword);
            cout << "\nFound " << results.size() << " book(s):\n";
            for (auto bookPtr : results) {
                bookPtr->displayBookInfo();
                cout << "---\n";
            }
            break;
        }
        
        case 3: { // Tìm sách theo tác giả
            string author = prompt("Enter author name: ");
            auto results = library.searchByAuthor(author);
            cout << "\nFound " << results.size() << " book(s):\n";
            for (auto bookPtr : results) {
                bookPtr->displayBookInfo();
                cout << "---\n";
            }
            break;
        }
        
        case 4: { // Xem sách theo danh mục
            library.displayBooksByCategory();
            break;
        }
        
        case 5: { // Xem sách phổ biến
            auto popularBooks = library.sortByPopularity();
            cout << "\n--- Popular Books (sorted by views) ---\n";
            for (auto bookPtr : popularBooks) {
                bookPtr->displayBookInfo();
                cout << "---\n";
            }
            break;
        }
        
        case 6: { // Mượn sách
            string bookId = prompt("Enter Book ID: ");
            Book* book = library.findBookById(bookId);
            
            if (!book) {
                cout << "Book not found!\n";
                break;
            }
            
            if (book->getAvailableQuantity() <= 0) {
                cout << "Book is not available. Consider reserving it.\n";
                break;
            }
            
            // Tạo Order mới
            string orderId = "ORD" + toString(nextOrderId++);
            string userId = toString(currentUser->getId());
            string borrowDate = getCurrentDate();
            string dueDate = calculateDueDate(14); // 14 ngày mượn
            
            Order newOrder(orderId, userId, bookId, "ISSUED", borrowDate, borrowDate, dueDate);
            orders.push_back(newOrder);
            
            // Giảm số sách có sẵn
            book->decreaseAvailableQuantity();
            
            cout << "Book borrowed successfully!\n";
            cout << "Order ID: " << orderId << "\n";
            cout << "Due date: " << dueDate << "\n";
            break;
        }
        
        case 7: { // Xem lịch sử mượn
            string userId = toString(currentUser->getId());
            cout << "\n--- My Borrow History ---\n";
            bool found = false;
            
            for (const auto &order : orders) {
                if (order.getUserId() == userId) {
                    order.displayOrderInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No borrow history.\n";
            break;
        }
        
        case 8: { // Gia hạn sách
            string orderId = prompt("Enter Order ID: ");
            bool found = false;
            
            for (auto &order : orders) {
                if (order.getOrderId() == orderId && 
                    order.getUserId() == toString(currentUser->getId())) {
                    
                    if (order.getStatus() == "ISSUED") {
                        order.renewOrder();
                        cout << "Loan renewed successfully!\n";
                        found = true;
                    } else {
                        cout << "Cannot renew this order (status: " << order.getStatus() << ")\n";
                        found = true;
                    }
                    break;
                }
            }
            
            if (!found) cout << "Order not found!\n";
            break;
        }
        
        case 9: { // Trả sách
            string orderId = prompt("Enter Order ID: ");
            bool found = false;
            
            for (auto &order : orders) {
                if (order.getOrderId() == orderId && 
                    order.getUserId() == toString(currentUser->getId())) {
                    
                    if (order.getStatus() == "ISSUED" || order.getStatus() == "OVERDUE") {
                        order.setReturnDate(getCurrentDate());
                        order.setStatus("RETURNED");
                        
                        // Tăng số sách có sẵn
                        Book* book = library.findBookById(order.getBookId());
                        if (book) book->increaseAvailableQuantity();
                        
                        cout << "Book returned successfully!\n";
                        found = true;
                    } else {
                        cout << "Cannot return this order (status: " << order.getStatus() << ")\n";
                        found = true;
                    }
                    break;
                }
            }
            
            if (!found) cout << "Order not found!\n";
            break;
        }
        
        case 10: { // Đặt trước sách
            string bookId = prompt("Enter Book ID to reserve: ");
            Book* book = library.findBookById(bookId);
            
            if (!book) {
                cout << "Book not found!\n";
                break;
            }
            
            string reservationId = "RES" + toString(nextReservationId++);
            string userId = toString(currentUser->getId());
            string reservationDate = getCurrentDate();
            string expiryDate = calculateDueDate(7); // Hết hạn sau 7 ngày
            
            Reservation newReservation(reservationId, userId, bookId, reservationDate, expiryDate);
            reservations.push_back(newReservation);
            
            cout << "Book reserved successfully!\n";
            cout << "Reservation ID: " << reservationId << "\n";
            cout << "Expiry date: " << expiryDate << "\n";
            break;
        }
        
        case 11: { // Xem danh sách đặt trước
            string userId = toString(currentUser->getId());
            cout << "\n--- My Reservations ---\n";
            bool found = false;
            
            for (const auto &reservation : reservations) {
                if (reservation.getUserId() == userId) {
                    reservation.displayReservationInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No reservations found.\n";
            break;
        }
        
        case 12: { // Yêu cầu sách mới
            string title = prompt("Book title: ");
            string author = prompt("Author: ");
            string isbn = prompt("ISBN (optional): ");
            string reason = prompt("Reason for request (optional): ");
            
            string requestId = "REQ" + toString(nextRequestId++);
            string userId = toString(currentUser->getId());
            string requestDate = getCurrentDate();
            
            BookRequest newRequest(requestId, userId, title, author, isbn, requestDate, reason);
            bookRequests.push_back(newRequest);
            
            cout << "Book request submitted successfully!\n";
            cout << "Request ID: " << requestId << "\n";
            break;
        }
        
        case 13: { // Xem yêu cầu của tôi
            string userId = toString(currentUser->getId());
            cout << "\n--- My Book Requests ---\n";
            bool found = false;
            
            for (const auto &request : bookRequests) {
                if (request.getUserId() == userId) {
                    request.displayRequestInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No requests found.\n";
            break;
        }
        
        case 14: { // Hủy yêu cầu
            string requestId = prompt("Enter Request ID to cancel: ");
            bool found = false;
            
            for (auto &request : bookRequests) {
                if (request.getRequestId() == requestId && 
                    request.getUserId() == toString(currentUser->getId())) {
                    
                    if (request.getStatus() == RequestStatus::Pending) {
                        request.cancelRequest();
                        cout << "Request cancelled successfully!\n";
                        found = true;
                    } else {
                        cout << "Cannot cancel this request (already processed).\n";
                        found = true;
                    }
                    break;
                }
            }
            
            if (!found) cout << "Request not found!\n";
            break;
        }
        
        case 15: { // Đánh giá sách
            string orderId = prompt("Enter Order ID: ");
            bool found = false;
            
            for (auto &order : orders) {
                if (order.getOrderId() == orderId && 
                    order.getUserId() == toString(currentUser->getId())) {
                    
                    if (order.getStatus() == "RETURNED") {
                        cout << "Rate (1-5 stars): ";
                        int rating;
                        cin >> rating;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        
                        string review = prompt("Review: ");
                        order.addReview(rating, review);
                        
                        cout << "Review added successfully!\n";
                        found = true;
                    } else {
                        cout << "Can only review returned books.\n";
                        found = true;
                    }
                    break;
                }
            }
            
            if (!found) cout << "Order not found!\n";
            break;
        }
        
        case 16: { // Xem hồ sơ
            currentUser->displayUserInfo();
            break;
        }
        
        case 17: { // Cập nhật hồ sơ
            cout << "\n--- Update Profile (leave blank to keep current value) ---\n";
            string name = prompt("Full name: ");
            string dob = prompt("Date of Birth (YYYY-MM-DD): ");
            
            cout << "Gender (1=Male, 2=Female, 3=Other): ";
            int genderChoice;
            cin >> genderChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Gender gender = (genderChoice == 1) ? Gender::Male : 
                           (genderChoice == 2) ? Gender::Female : Gender::Other;
            
            string address = prompt("Address: ");
            string phone = prompt("Phone: ");
            
            if (userService.updateUserProfile(currentUser->getId(), name, dob, gender, address, phone)) {
                cout << "Profile updated successfully!\n";
            } else {
                cout << "Failed to update profile.\n";
            }
            break;
        }
        
        case 18: { // Đổi mật khẩu
            string oldPassword = prompt("Old password: ");
            string newPassword = prompt("New password: ");
            
            if (userService.changePassword(currentUser->getId(), oldPassword, newPassword)) {
                cout << "Password changed successfully!\n";
            } else {
                cout << "Failed to change password. Check your old password.\n";
            }
            break;
        }
        
        case 0:
            running = false;
            cout << "Logged out successfully!\n";
            break;
            
        default:
            cout << "Invalid choice!\n";
            break;
        }
    }
}

// ===== LIBRARIAN MENU =====
// Chức năng dành cho thủ thư (Librarian):
// - Tất cả chức năng của Reader
// - Thêm/sửa/xóa sách
// - Xử lý mượn/trả sách cho người dùng
// - Duyệt yêu cầu sách
// - Tạo báo cáo cơ bản
void librarianMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== LIBRARIAN MENU (" << currentUser->getFullName() << ") ==========\n";
        cout << "--- Book Management ---\n";
        cout << "1. Add new book\n";
        cout << "2. Update book\n";
        cout << "3. Remove book\n";
        cout << "4. View all books\n";
        cout << "5. Search books\n";
        cout << "\n--- Borrowing Management ---\n";
        cout << "6. Issue book to user\n";
        cout << "7. Process book return\n";
        cout << "8. View all active loans\n";
        cout << "9. View overdue loans\n";
        cout << "\n--- Request Management ---\n";
        cout << "10. View pending book requests\n";
        cout << "11. Approve book request\n";
        cout << "12. Reject book request\n";
        cout << "\n--- Reports ---\n";
        cout << "13. Generate borrowing report\n";
        cout << "14. Generate inventory report\n";
        cout << "\n--- Personal ---\n";
        cout << "15. View my profile\n";
        cout << "16. Access Reader features\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Thêm sách mới
            string title = prompt("Title: ");
            string author = prompt("Author: ");
            string publisher = prompt("Publisher: ");
            
            cout << "Publication year: ";
            int year;
            cin >> year;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            string isbn = prompt("ISBN: ");
            string category = prompt("Category: ");
            string keyword = prompt("Keywords (comma separated): ");
            
            cout << "Quantity: ";
            int quantity;
            cin >> quantity;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Pages: ";
            int pages;
            cin >> pages;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            string briefDesc = prompt("Brief description: ");
            string detailedDesc = prompt("Detailed description: ");
            
            cout << "Rental price: ";
            double rentalPrice;
            cin >> rentalPrice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Purchase price: ";
            double purchasePrice;
            cin >> purchasePrice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            vector<string> categories = {category};
            vector<string> keywords = {keyword};
            
            library.addNewBook(title, author, publisher, year, isbn, categories, keywords, 
                             quantity, pages, briefDesc, detailedDesc, rentalPrice, purchasePrice);
            
            cout << "Book added successfully!\n";
            break;
        }
        
        case 2: { // Cập nhật sách
            string bookId = prompt("Enter Book ID to update: ");
            Book* book = library.findBookById(bookId);
            
            if (!book) {
                cout << "Book not found!\n";
                break;
            }
            
            cout << "\n--- Update Book (leave blank to keep current) ---\n";
            string title = prompt("Title: ");
            string author = prompt("Author: ");
            string publisher = prompt("Publisher: ");
            
            // Simplified update - chỉ cập nhật title, author, publisher
            if (!title.empty() || !author.empty() || !publisher.empty()) {
                library.updateBook(bookId, 
                    title.empty() ? book->getTitle() : title,
                    author.empty() ? book->getAuthor() : author,
                    publisher.empty() ? book->getPublisher() : publisher,
                    book->getYearOfPublication(),
                    book->getCategories(),
                    book->getKeywords(),
                    book->getTotalQuantity(),
                    book->getTotalPages(),
                    book->getBriefDescription(),
                    book->getDetailedDescription(),
                    book->getRentalPrice(),
                    book->getPurchasePrice());
                
                cout << "Book updated successfully!\n";
            }
            break;
        }
        
        case 3: { // Xóa sách
            string bookId = prompt("Enter Book ID to remove: ");
            
            if (library.removeBook(bookId)) {
                cout << "Book removed successfully!\n";
            } else {
                cout << "Failed to remove book. Book not found.\n";
            }
            break;
        }
        
        case 4: { // Xem tất cả sách
            library.displayAllBooks();
            break;
        }
        
        case 5: { // Tìm kiếm sách
            cout << "Search by: 1=Title, 2=Author, 3=Keyword: ";
            int searchChoice;
            cin >> searchChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            string query = prompt("Enter search term: ");
            vector<Book*> results;
            
            if (searchChoice == 1) results = library.searchByTitle(query);
            else if (searchChoice == 2) results = library.searchByAuthor(query);
            else if (searchChoice == 3) results = library.searchByKeyword(query);
            
            cout << "\nFound " << results.size() << " book(s):\n";
            for (auto bookPtr : results) {
                bookPtr->displayBookInfo();
                cout << "---\n";
            }
            break;
        }
        
        case 6: { // Cho mượn sách
            string userEmail = prompt("User email: ");
            User* user = userService.findUserByEmail(userEmail);
            
            if (!user) {
                cout << "User not found!\n";
                break;
            }
            
            string bookId = prompt("Book ID: ");
            Book* book = library.findBookById(bookId);
            
            if (!book) {
                cout << "Book not found!\n";
                break;
            }
            
            if (book->getAvailableQuantity() <= 0) {
                cout << "Book is not available!\n";
                break;
            }
            
            string orderId = "ORD" + toString(nextOrderId++);
            string userId = toString(user->getId());
            string borrowDate = getCurrentDate();
            string dueDate = calculateDueDate(14);
            
            Order newOrder(orderId, userId, bookId, "ISSUED", borrowDate, borrowDate, dueDate);
            orders.push_back(newOrder);
            book->decreaseAvailableQuantity();
            
            cout << "Book issued successfully!\n";
            cout << "Order ID: " << orderId << "\n";
            break;
        }
        
        case 7: { // Xử lý trả sách
            string orderId = prompt("Enter Order ID: ");
            bool found = false;
            
            for (auto &order : orders) {
                if (order.getOrderId() == orderId) {
                    if (order.getStatus() == "ISSUED" || order.getStatus() == "OVERDUE") {
                        order.setReturnDate(getCurrentDate());
                        order.setStatus("RETURNED");
                        
                        Book* book = library.findBookById(order.getBookId());
                        if (book) book->increaseAvailableQuantity();
                        
                        cout << "Book return processed successfully!\n";
                        found = true;
                    } else {
                        cout << "Order status: " << order.getStatus() << "\n";
                        found = true;
                    }
                    break;
                }
            }
            
            if (!found) cout << "Order not found!\n";
            break;
        }
        
        case 8: { // Xem đơn mượn đang hoạt động
            cout << "\n--- Active Loans ---\n";
            bool found = false;
            
            for (const auto &order : orders) {
                if (order.getStatus() == "ISSUED") {
                    order.displayOrderInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No active loans.\n";
            break;
        }
        
        case 9: { // Xem đơn quá hạn
            cout << "\n--- Overdue Loans ---\n";
            bool found = false;
            
            for (const auto &order : orders) {
                if (order.isOverdue() || order.getStatus() == "OVERDUE") {
                    order.displayOrderInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No overdue loans.\n";
            break;
        }
        
        case 10: { // Xem yêu cầu đang chờ
            cout << "\n--- Pending Book Requests ---\n";
            bool found = false;
            
            for (const auto &request : bookRequests) {
                if (request.getStatus() == RequestStatus::Pending) {
                    request.displayRequestInfo();
                    cout << "---\n";
                    found = true;
                }
            }
            
            if (!found) cout << "No pending requests.\n";
            break;
        }
        
        case 11: { // Duyệt yêu cầu
            string requestId = prompt("Enter Request ID to approve: ");
            bool found = false;
            
            for (auto &request : bookRequests) {
                if (request.getRequestId() == requestId) {
                    string notes = prompt("Approval notes (optional): ");
                    request.approveRequest(getCurrentDate(), notes);
                    cout << "Request approved!\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) cout << "Request not found!\n";
            break;
        }
        
        case 12: { // Từ chối yêu cầu
            string requestId = prompt("Enter Request ID to reject: ");
            bool found = false;
            
            for (auto &request : bookRequests) {
                if (request.getRequestId() == requestId) {
                    string notes = prompt("Rejection reason: ");
                    request.rejectRequest(notes);
                    cout << "Request rejected!\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) cout << "Request not found!\n";
            break;
        }
        
        case 13: { // Báo cáo mượn sách
            ReportGenerator::borrowingTrends(orders);
            break;
        }
        
        case 14: { // Báo cáo tồn kho
            ReportGenerator::inventory(library.getAllBooks());
            break;
        }
        
        case 15: { // Xem hồ sơ
            currentUser->displayUserInfo();
            break;
        }
        
        case 16: { // Truy cập chức năng Reader
            readerMenu(currentUser, library, userService);
            break;
        }
        
        case 0:
            running = false;
            cout << "Logged out successfully!\n";
            break;
            
        default:
            cout << "Invalid choice!\n";
            break;
        }
    }
}

// ===== ADMIN MENU =====
// Chức năng dành cho quản trị viên (Admin):
// - Tất cả chức năng của Librarian và Reader
// - Tạo tài khoản Librarian
// - Quản lý người dùng (xem, xóa, kích hoạt)
// - Tạo báo cáo tổng hợp
// - Backup và restore hệ thống
void adminMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== ADMIN MENU (" << currentUser->getFullName() << ") ==========\n";
        cout << "--- User Management ---\n";
        cout << "1. Create Librarian account\n";
        cout << "2. View all users\n";
        cout << "3. View users by role\n";
        cout << "4. Deactivate user\n";
        cout << "5. Activate user\n";
        cout << "\n--- System Reports ---\n";
        cout << "6. General statistics\n";
        cout << "7. User list report\n";
        cout << "8. Most borrowed books\n";
        cout << "9. Most active users\n";
        cout << "10. Popular categories\n";
        cout << "11. Overdue books report\n";
        cout << "\n--- Data Management ---\n";
        cout << "12. Backup system\n";
        cout << "13. Restore system\n";
        cout << "\n--- Personal ---\n";
        cout << "14. View my profile\n";
        cout << "15. Access Librarian features\n";
        cout << "16. Access Reader features\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Tạo tài khoản Librarian
            string name = prompt("Full name: ");
            string dob = prompt("Date of Birth (YYYY-MM-DD): ");
            
            cout << "Gender (1=Male, 2=Female, 3=Other): ";
            int genderChoice;
            cin >> genderChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Gender gender = (genderChoice == 1) ? Gender::Male : 
                           (genderChoice == 2) ? Gender::Female : Gender::Other;
            
            string address = prompt("Address: ");
            string phone = prompt("Phone: ");
            string email = prompt("Email: ");
            string password = prompt("Password: ");
            
            if (userService.registerUser(name, dob, gender, address, phone, email, password, Role::Librarian)) {
                cout << "Librarian account created successfully!\n";
            } else {
                cout << "Failed to create account. Email may already exist.\n";
            }
            break;
        }
        
        case 2: { // Xem tất cả người dùng
            userService.displayAllUsers();
            break;
        }
        
        case 3: { // Xem người dùng theo vai trò
            cout << "Select role: 1=Reader, 2=Librarian, 3=Admin: ";
            int roleChoice;
            cin >> roleChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            Role role = (roleChoice == 1) ? Role::Reader : 
                       (roleChoice == 2) ? Role::Librarian : Role::Admin;
            
            auto users = userService.getUsersByRole(role);
            cout << "\n--- Users with role " << roleChoice << " ---\n";
            for (auto userPtr : users) {
                userPtr->displayUserInfo();
                cout << "---\n";
            }
            break;
        }
        
        case 4: { // Vô hiệu hóa người dùng
            string email = prompt("User email to deactivate: ");
            User* user = userService.findUserByEmail(email);
            
            if (!user) {
                cout << "User not found!\n";
                break;
            }
            
            if (userService.deactivateUser(user->getId())) {
                cout << "User deactivated successfully!\n";
            } else {
                cout << "Failed to deactivate user.\n";
            }
            break;
        }
        
        case 5: { // Kích hoạt người dùng
            string email = prompt("User email to activate: ");
            User* user = userService.findUserByEmail(email);
            
            if (!user) {
                cout << "User not found!\n";
                break;
            }
            
            if (userService.activateUser(user->getId())) {
                cout << "User activated successfully!\n";
            } else {
                cout << "Failed to activate user.\n";
            }
            break;
        }
        
        case 6: { // Thống kê tổng quan
            auto users = userService.getAllUsers();
            vector<User> userVector;
            for (auto userPtr : users) {
                userVector.push_back(*userPtr);
            }
            
            ReportGenerator::generalStats(userVector, library.getAllBooks(), orders);
            break;
        }
        
        case 7: { // Báo cáo danh sách người dùng
            auto users = userService.getAllUsers();
            vector<User> userVector;
            for (auto userPtr : users) {
                userVector.push_back(*userPtr);
            }
            
            ReportGenerator::userList(userVector);
            break;
        }
        
        case 8: { // Sách được mượn nhiều nhất
            ReportGenerator::mostBorrowedBooks(library.getAllBooks(), orders, 10);
            break;
        }
        
        case 9: { // Người dùng tích cực nhất
            auto users = userService.getAllUsers();
            vector<User> userVector;
            for (auto userPtr : users) {
                userVector.push_back(*userPtr);
            }
            
            ReportGenerator::mostActiveUsers(userVector, orders, 10);
            break;
        }
        
        case 10: { // Danh mục phổ biến
            ReportGenerator::popularCategories(library.getAllBooks(), orders);
            break;
        }
        
        case 11: { // Báo cáo sách quá hạn
            auto users = userService.getAllUsers();
            vector<User> userVector;
            for (auto userPtr : users) {
                userVector.push_back(*userPtr);
            }
            
            ReportGenerator::overdueBooks(orders, library.getAllBooks(), userVector);
            break;
        }
        
        case 12: { // Backup hệ thống
            string filename = prompt("Backup filename (default: backup.txt): ");
            if (filename.empty()) filename = "backup.txt";
            
            if (library.saveToFile(filename + "_books") && 
                userService.saveToFile(filename + "_users")) {
                cout << "System backed up successfully!\n";
                cout << "Files: " << filename << "_books, " << filename << "_users\n";
            } else {
                cout << "Backup failed!\n";
            }
            break;
        }
        
        case 13: { // Restore hệ thống
            string filename = prompt("Backup filename to restore: ");
            
            if (library.loadFromFile(filename + "_books") && 
                userService.loadFromFile(filename + "_users")) {
                cout << "System restored successfully!\n";
            } else {
                cout << "Restore failed! Files may not exist.\n";
            }
            break;
        }
        
        case 14: { // Xem hồ sơ
            currentUser->displayUserInfo();
            break;
        }
        
        case 15: { // Truy cập chức năng Librarian
            librarianMenu(currentUser, library, userService);
            break;
        }
        
        case 16: { // Truy cập chức năng Reader
            readerMenu(currentUser, library, userService);
            break;
        }
        
        case 0:
            running = false;
            cout << "Logged out successfully!\n";
            break;
            
        default:
            cout << "Invalid choice!\n";
            break;
        }
    }
}

// ===== MAIN FUNCTION =====
// Hàm chính: Khởi tạo hệ thống và vòng lặp menu
int main() {
    // Khởi tạo các đối tượng quản lý
    UserService userService;
    Library library;
    
    // Load dữ liệu từ file (nếu có)
    bool dataLoaded = userService.loadFromFile("users.txt");
    library.loadFromFile("books.txt");
    
    // Nếu chưa có dữ liệu, tạo sample accounts
    if (!dataLoaded || userService.getAllUsers().empty()) {
        cout << "Creating sample accounts...\n";
        
        // Tạo Admin account
        userService.registerUser("Nguyen Van Admin", "1990-01-01", Gender::Male,
                                "123 Admin St", "0901234567", "admin@library.com",
                                "admin123", Role::Admin);
        
        // Tạo Librarian accounts
        userService.registerUser("Tran Thi Thu", "1995-05-15", Gender::Female,
                                "456 Librarian Ave", "0912345678", "librarian1@library.com",
                                "lib123", Role::Librarian);
        
        userService.registerUser("Le Van Binh", "1993-03-20", Gender::Male,
                                "789 Librarian Blvd", "0923456789", "librarian2@library.com",
                                "lib123", Role::Librarian);
        
        // Tạo Reader accounts
        userService.registerUser("Pham Minh Hieu", "2000-07-10", Gender::Male,
                                "111 Reader St", "0934567890", "reader1@library.com",
                                "read123", Role::Reader);
        
        userService.registerUser("Hoang Thi Lan", "1998-12-25", Gender::Female,
                                "222 Reader Ave", "0945678901", "reader2@library.com",
                                "read123", Role::Reader);
        
        userService.registerUser("Vo Quoc Bao", "2001-09-08", Gender::Male,
                                "333 Reader Blvd", "0956789012", "reader3@library.com",
                                "read123", Role::Reader);
        
        cout << "Sample accounts created successfully!\n";
        cout << "Admin: admin@library.com / admin123\n";
        cout << "Librarian: librarian1@library.com / lib123\n";
        cout << "Reader: reader1@library.com / read123\n\n";
    }
    
    User* currentUser = nullptr;
    bool running = true;
    
    cout << "========================================\n";
    cout << "  LIBRARY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    
    while (running) {
        if (!currentUser) {
            // Menu chính khi chưa đăng nhập
            cout << "\n========== MAIN MENU ==========\n";
            cout << "1. Login\n";
            cout << "2. Register (Reader only)\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1: { // Đăng nhập
                string email = prompt("Email: ");
                string password = prompt("Password: ");
                
                currentUser = userService.login(email, password);
                
                if (currentUser) {
                    cout << "Login successful! Welcome, " << currentUser->getFullName() << "!\n";
                } else {
                    cout << "Login failed! Invalid email or password.\n";
                }
                break;
            }
            
            case 2: { // Đăng ký (chỉ Reader)
                string name = prompt("Full name: ");
                string dob = prompt("Date of Birth (YYYY-MM-DD): ");
                
                cout << "Gender (1=Male, 2=Female, 3=Other): ";
                int genderChoice;
                cin >> genderChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Gender gender = (genderChoice == 1) ? Gender::Male : 
                               (genderChoice == 2) ? Gender::Female : Gender::Other;
                
                string address = prompt("Address: ");
                string phone = prompt("Phone: ");
                string email = prompt("Email: ");
                string password = prompt("Password: ");
                
                if (userService.registerUser(name, dob, gender, address, phone, email, password)) {
                    cout << "Registration successful! Please login.\n";
                } else {
                    cout << "Registration failed! Email may already exist.\n";
                }
                break;
            }
            
            case 0:
                running = false;
                break;
                
            default:
                cout << "Invalid choice!\n";
                break;
            }
        } else {
            // Điều hướng theo vai trò
            Role role = currentUser->getRole();
            
            if (role == Role::Admin) {
                adminMenu(currentUser, library, userService);
                currentUser = nullptr;
            } else if (role == Role::Librarian) {
                librarianMenu(currentUser, library, userService);
                currentUser = nullptr;
            } else { // Reader
                readerMenu(currentUser, library, userService);
                currentUser = nullptr;
            }
        }
    }
    
    // Lưu dữ liệu trước khi thoát
    userService.saveToFile("users.txt");
    library.saveToFile("books.txt");
    
    cout << "\n========================================\n";
    cout << "  Thank you for using our system!\n";
    cout << "  Goodbye!\n";
    cout << "========================================\n";
    
    return 0;
}


