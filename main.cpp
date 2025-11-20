// File main - chương trình quản lý thư viện
// Xử lý login, menu chính và điều hướng theo role
// Quản lý: User, Book, Order, Reservation, BookRequest

#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>

#include "UserManagement/User.h"
#include "UserManagement/Services/UserService.h"
#include "BookManagement/Library.h"
#include "BookManagement/Book.h"
#include "BorrowingProcess/Models/Order.h"
#include "BorrowingProcess/Models/Reservation.h"
#include "BorrowingProcess/Models/BookRequest.h"
#include "AdminAndReporting/Reports/ReportGenerator.h"

using namespace std;

// Biến toàn cục - lưu data trong runtime
vector<Order> orders;
vector<Reservation> reservations;
vector<BookRequest> bookRequests;
int nextOrderId = 1;
int nextReservationId = 1;
int nextRequestId = 1;

// Hàm tự động lưu file sau mỗi thao tác
void autoSave(Library& library, UserService& userService) {
    library.saveToFile("books.txt");
    userService.saveToFile("users.txt");
    
    ofstream ordersFile("orders.txt");
    if (ordersFile.is_open()) {
        for (const Order& order : orders) {
            ordersFile << order.getOrderId() << "|"
                      << order.getUserId() << "|"
                      << order.getBookId() << "|"
                      << order.getStatus() << "|"
                      << order.getBorrowDate() << "|"
                      << order.getIssueDate() << "|"
                      << order.getDueDate() << "|"
                      << order.getReturnDate() << "|"
                      << order.getRenewalPeriod() << "|"
                      << order.getBookCondition() << "\n";
        }
        ordersFile.close();
    }
    
    ofstream reservationsFile("database/reservations.txt");
    if (reservationsFile.is_open()) {
        for (const Reservation& res : reservations) {
            reservationsFile << res.getReservationId() << "|"
                           << res.getUserId() << "|"
                           << res.getBookId() << "|"
                           << res.statusToString() << "|"
                           << res.getReservationDate() << "\n";
        }
        reservationsFile.close();
    }
    
    ofstream requestsFile("database/bookrequests.txt");
    if (requestsFile.is_open()) {
        for (const BookRequest& req : bookRequests) {
            requestsFile << req.getRequestId() << "|"
                        << req.getUserId() << "|"
                        << req.getBookTitle() << "|"
                        << req.getBookAuthor() << "|"
                        << req.statusToString() << "|"
                        << req.getRequestDate() << "\n";
        }
        requestsFile.close();
    }
}

// Đọc dữ liệu orders từ file
void loadOrders() {
    ifstream ordersFile("database/orders.txt");
    if (ordersFile.is_open()) {
        string line;
        while (getline(ordersFile, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string orderId, userId, bookId, status, borrowDate, issueDate, dueDate, returnDate, bookCondition;
            string renewalPeriodStr;
            
            getline(ss, orderId, '|');
            getline(ss, userId, '|');
            getline(ss, bookId, '|');
            getline(ss, status, '|');
            getline(ss, borrowDate, '|');
            getline(ss, issueDate, '|');
            getline(ss, dueDate, '|');
            getline(ss, returnDate, '|');
            getline(ss, renewalPeriodStr, '|');
            getline(ss, bookCondition);
            
            int renewalPeriod = renewalPeriodStr.empty() ? 0 : stoi(renewalPeriodStr);
            
            Order order(orderId, userId, bookId, status, borrowDate, issueDate, dueDate, returnDate, renewalPeriod, bookCondition);
            orders.push_back(order);
            
            // Cập nhật nextOrderId
            if (!orderId.empty() && orderId.length() > 1 && orderId[0] == 'O') {
                int id = stoi(orderId.substr(3));
                if (id >= nextOrderId) nextOrderId = id + 1;
            }
        }
        ordersFile.close();
    }
}

// Đọc dữ liệu reservations từ file
void loadReservations() {
    ifstream reservationsFile("database/reservations.txt");
    if (reservationsFile.is_open()) {
        string line;
        while (getline(reservationsFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            
            stringstream ss(line);
            string resId, userId, bookId, statusStr, resDate;
            
            getline(ss, resId, '|');
            getline(ss, userId, '|');
            getline(ss, bookId, '|');
            getline(ss, statusStr, '|');
            getline(ss, resDate);
            
            Reservation res(resId, userId, bookId, resDate, "");
            res.setStatus(Reservation::stringToStatus(statusStr));
            reservations.push_back(res);
            
            // Update nextReservationId
            if (!resId.empty() && resId.length() > 3 && resId[0] == 'R') {
                int id = stoi(resId.substr(3)); // "RES1" -> skip "RES", get "1"
                if (id >= nextReservationId) nextReservationId = id + 1;
            }
        }
        reservationsFile.close();
    }
}

// Đọc dữ liệu book requests từ file
void loadBookRequests() {
    ifstream requestsFile("database/bookrequests.txt");
    if (requestsFile.is_open()) {
        string line;
        while (getline(requestsFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            
            stringstream ss(line);
            string reqId, userId, bookTitle, author, statusStr, reqDate;
            
            getline(ss, reqId, '|');
            getline(ss, userId, '|');
            getline(ss, bookTitle, '|');
            getline(ss, author, '|');
            getline(ss, statusStr, '|');
            getline(ss, reqDate);
            
            BookRequest req(reqId, userId, bookTitle, author, "", reqDate);
            req.setStatus(BookRequest::stringToStatus(statusStr));
            bookRequests.push_back(req);
            
            // Update nextRequestId
            if (!reqId.empty() && reqId.length() > 3 && reqId[0] == 'R') {
                int id = stoi(reqId.substr(3)); // "REQ1" -> skip "REQ", get "1"
                if (id >= nextRequestId) nextRequestId = id + 1;
            }
        }
        requestsFile.close();
    }
}

// Các hàm tiện ích

// Nhận input từ user
string prompt(const string &message) {
    cout << message;
    string value;
    getline(cin, value);
    return value;
}

// Chuyển int sang string
string toString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// Lấy ngày hiện tại
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", 
            1900 + ltm->tm_year, 
            1 + ltm->tm_mon, 
            ltm->tm_mday);
    return string(buffer);
}

// Tính ngày đến hạn (cộng thêm số ngày)
string calculateDueDate(int days) {
    time_t now = time(0);
    now += days * 24 * 60 * 60; // Thêm số giây
    tm* ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", 
            1900 + ltm->tm_year, 
            1 + ltm->tm_mon, 
            ltm->tm_mday);
    return string(buffer);
}

// Menu dành cho User (Reader)
void readerMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== USER MENU (" << currentUser->getName() << ") ==========\n";
        cout << "1. Browse & Search books\n";
        cout << "2. Manage reservations\n";
        cout << "3. Manage book requests\n";
        cout << "4. My profile\n";
        cout << "5. Change password\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Browse & Search books - submenu
            cout << "\n--- Browse & Search ---\n";
            cout << "1. View available books\n";
            cout << "2. Search by title\n";
            cout << "3. Search by author\n";
            cout << "4. View by category\n";
            cout << "5. View popular books\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) {
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
            } else if (subChoice == 2) {
                string keyword = prompt("Enter book title: ");
                auto results = library.searchByTitle(keyword);
                cout << "\nFound " << results.size() << " book(s):\n";
                for (auto bookPtr : results) {
                    bookPtr->displayBookInfo();
                    cout << "---\n";
                }
            } else if (subChoice == 3) {
                string author = prompt("Enter author name: ");
                auto results = library.searchByAuthor(author);
                cout << "\nFound " << results.size() << " book(s):\n";
                for (auto bookPtr : results) {
                    bookPtr->displayBookInfo();
                    cout << "---\n";
                }
            } else if (subChoice == 4) {
                library.displayBooksByCategory();
            } else if (subChoice == 5) {
                auto popularBooks = library.sortByPopularity();
                cout << "\n--- Popular Books (sorted by views) ---\n";
                for (auto bookPtr : popularBooks) {
                    bookPtr->displayBookInfo();
                    cout << "---\n";
                }
            }
            break;
        }
        
        case 2: { // Quản lý đặt trước
            cout << "\n--- Manage Reservations ---\n";
            cout << "1. Reserve a book\n";
            cout << "2. View my reservations\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Đặt trước sách
                string bookId = prompt("Enter Book ID to reserve: ");
                Book* book = library.findBookById(bookId);
                
                if (!book) {
                    cout << "Book not found!\n";
                } else {
                    string reservationId = "RES" + toString(nextReservationId++);
                    string userId = toString(currentUser->getId());
                    string reservationDate = getCurrentDate();
                    string expiryDate = calculateDueDate(7);
                    
                    Reservation newReservation(reservationId, userId, bookId, reservationDate, expiryDate);
                    reservations.push_back(newReservation);
                    autoSave(library, userService);
                    
                    cout << "Book reserved successfully!\n";
                    cout << "Reservation ID: " << reservationId << "\n";
                    cout << "Expiry date: " << expiryDate << "\n";
                }
            } else if (subChoice == 2) { // Xem danh sách đặt trước
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
            }
            break;
        }
        
        case 3: { // Quản lý yêu cầu sách
            cout << "\n--- Manage Book Requests ---\n";
            cout << "1. Submit new book request\n";
            cout << "2. View my requests\n";
            cout << "3. Cancel a request\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Yêu cầu sách mới
                string title = prompt("Book title: ");
                string author = prompt("Author: ");
                string isbn = prompt("ISBN (optional): ");
                string reason = prompt("Reason for request (optional): ");
                
                string requestId = "REQ" + toString(nextRequestId++);
                string userId = toString(currentUser->getId());
                string requestDate = getCurrentDate();
                
                BookRequest newRequest(requestId, userId, title, author, isbn, requestDate, reason);
                bookRequests.push_back(newRequest);
                autoSave(library, userService);
                
                cout << "Book request submitted successfully!\n";
                cout << "Request ID: " << requestId << "\n";
            } else if (subChoice == 2) { // Xem yêu cầu của tôi
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
            } else if (subChoice == 3) { // Hủy yêu cầu
                string requestId = prompt("Enter Request ID to cancel: ");
                bool found = false;
                
                for (auto &request : bookRequests) {
                    if (request.getRequestId() == requestId && 
                        request.getUserId() == toString(currentUser->getId())) {
                        
                        if (request.getStatus() == RequestStatus::Pending) {
                            request.cancelRequest();
                            autoSave(library, userService);
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
            }
            break;
        }
        
        case 4: { // Xem & cập nhật hồ sơ
            cout << "\n--- My Profile ---\n";
            currentUser->displayUserInfo();
            cout << "\nDo you want to update profile? (y/n): ";
            char updateChoice;
            cin >> updateChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (updateChoice == 'y' || updateChoice == 'Y') {
                cout << "\n--- Update Profile ---\n";
                string name = prompt("Name: ");
                string email = prompt("Email: ");
                string phone = prompt("Phone Number: ");
                
                if (userService.updateUserProfile(currentUser->getId(), name, email, phone)) {
                    autoSave(library, userService);
                    cout << "Profile updated successfully!\n";
                } else {
                    cout << "Failed to update profile.\n";
                }
            }
            break;
        }
        
        case 5: { // Đổi mật khẩu
            string oldPassword = prompt("Old password: ");
            string newPassword = prompt("New password: ");
            
            if (userService.changePassword(currentUser->getId(), oldPassword, newPassword)) {
                autoSave(library, userService);
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

// Menu dành cho Librarian (thủ thư)
void librarianMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== LIBRARIAN MENU (" << currentUser->getName() << ") ==========\n";
        cout << "1. Manage Books\n";
        cout << "2. Manage Orders\n";
        cout << "3. Manage Requests\n";
        cout << "4. Generate Reports\n";
        cout << "5. Access User Features\n";
        cout << "6. My Profile\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Quản lý sách
            cout << "\n--- Manage Books ---\n";
            cout << "1. Add new book\n";
            cout << "2. Update book\n";
            cout << "3. Remove book\n";
            cout << "4. View all books\n";
            cout << "5. Search books\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Thêm sách mới
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
                
                vector<string> categories = {category};
                vector<string> keywords = {keyword};
                
                library.addNewBook(title, author, publisher, year, isbn, categories, keywords, 
                                 quantity, pages, briefDesc, detailedDesc, rentalPrice);
                autoSave(library, userService);
                cout << "Book added successfully!\n";
            } else if (subChoice == 2) { // Cập nhật sách
                string bookId = prompt("Enter Book ID to update: ");
                Book* book = library.findBookById(bookId);
                
                if (!book) {
                    cout << "Book not found!\n";
                } else {
                    cout << "\n--- Update Book (leave blank to keep current) ---\n";
                    string title = prompt("Title: ");
                    string author = prompt("Author: ");
                    string publisher = prompt("Publisher: ");
                    
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
                            book->getRentalPrice());
                        autoSave(library, userService);
                        cout << "Book updated successfully!\n";
                    }
                }
            } else if (subChoice == 3) { // Xóa sách
                string bookId = prompt("Enter Book ID to remove: ");
                
                if (library.removeBook(bookId)) {
                    autoSave(library, userService);
                    cout << "Book removed successfully!\n";
                } else {
                    cout << "Failed to remove book. Book not found.\n";
                }
            } else if (subChoice == 4) { // Xem tất cả sách
                library.displayAllBooks();
            } else if (subChoice == 5) { // Tìm kiếm sách
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
            }
            break;
        }
        
        case 2: { // Quản lý cho mượn/trả
            cout << "\n--- Manage Loans ---\n";
            cout << "1. Issue book to user\n";
            cout << "2. Process book return\n";
            cout << "3. Renew loan for user\n";
            cout << "4. View all orders\n";
            cout << "5. View active loans\n";
            cout << "6. View overdue loans\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Cho mượn sách
                string userEmail = prompt("User email: ");
                User* user = userService.findUserByEmail(userEmail);
                
                if (!user) {
                    cout << "User not found!\n";
                } else {
                    string bookId = prompt("Book ID: ");
                    Book* book = library.findBookById(bookId);
                    
                    if (!book) {
                        cout << "Book not found!\n";
                    } else if (book->getAvailableQuantity() <= 0) {
                        cout << "Book is not available!\n";
                    } else {
                        string orderId = "ORD" + toString(nextOrderId++);
                        string userId = toString(user->getId());
                        string borrowDate = getCurrentDate();
                        string dueDate = calculateDueDate(14);
                        
                        Order newOrder(orderId, userId, bookId, "ISSUED", borrowDate, borrowDate, dueDate, "", 0, "GOOD");
                        orders.push_back(newOrder);
                        book->decreaseAvailableQuantity();
                        autoSave(library, userService);
                        
                        cout << "Book issued successfully!\n";
                        cout << "Order ID: " << orderId << "\n";
                    }
                }
            } else if (subChoice == 2) { // Xử lý trả sách
                string orderId = prompt("Enter Order ID: ");
                bool found = false;
                
                for (auto &order : orders) {
                    if (order.getOrderId() == orderId) {
                        if (order.getStatus() == "ISSUED" || order.getStatus() == "OVERDUE") {
                            cout << "Book condition (1=GOOD, 2=DAMAGED, 3=LOST): ";
                            int condChoice;
                            cin >> condChoice;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            
                            string condition = (condChoice == 2) ? "DAMAGED" : 
                                             (condChoice == 3) ? "LOST" : "GOOD";
                            order.setBookCondition(condition);
                            order.setReturnDate(getCurrentDate());
                            order.setStatus("RETURNED");
                            
                            Book* book = library.findBookById(order.getBookId());
                            if (book) {
                                book->increaseAvailableQuantity();
                                autoSave(library, userService);
                                
                                double fine = order.calculateFine(book->getRentalPrice());
                                double estimatedBookValue = book->getRentalPrice() * 100;
                                
                                cout << "\n=== Return Summary ===\n";
                                cout << "Book return processed successfully!\n";
                                cout << "Book condition: " << condition << "\n";
                                
                                if (fine > 0) {
                                    cout << "--- Fine Details ---\n";
                                    int overdueDays = order.calculateOverdueDays();
                                    if (overdueDays > 0) {
                                        cout << "Overdue days: " << overdueDays << "\n";
                                        cout << "Daily fine rate: " << book->getRentalPrice() << " VND\n";
                                        cout << "Overdue fine: " << (overdueDays * book->getRentalPrice()) << " VND\n";
                                    }
                                    if (condition == "DAMAGED") {
                                        cout << "Damaged book compensation (50%): " << (estimatedBookValue * 0.5) << " VND\n";
                                    } else if (condition == "LOST") {
                                        cout << "Lost book compensation (100%): " << estimatedBookValue << " VND\n";
                                    }
                                    cout << "-------------------\n";
                                    cout << "TOTAL FINE: " << fine << " VND\n";
                                } else {
                                    cout << "No fines. Thank you!\n";
                                }
                            }
                            
                            found = true;
                        } else {
                            cout << "Order status: " << order.getStatus() << "\n";
                            found = true;
                        }
                        break;
                    }
                }
                
                if (!found) cout << "Order not found!\n";
            } else if (subChoice == 3) { // Gia hạn sách cho user (tối đa 2 lần)
                string orderId = prompt("Enter Order ID: ");
                bool found = false;
                
                for (auto &order : orders) {
                    if (order.getOrderId() == orderId) {
                        if (order.getStatus() == "ISSUED") {
                            if (order.getRenewalPeriod() >= 2) {
                                cout << "Cannot renew! Maximum renewal limit (2 times) reached.\n";
                                cout << "User must return the book.\n";
                                found = true;
                            } else if (order.isOverdue()) {
                                cout << "Cannot renew overdue book! Please process return first.\n";
                                found = true;
                            } else {
                                order.renewOrder();
                                // Gia hạn thêm 14 ngày
                                string newDueDate = calculateDueDate(14);
                                order.setDueDate(newDueDate);
                                autoSave(library, userService);
                                cout << "Loan renewed successfully!\n";
                                cout << "Order ID: " << orderId << "\n";
                                cout << "New due date: " << newDueDate << "\n";
                                cout << "Renewals used: " << order.getRenewalPeriod() << "/2\n";
                                found = true;
                            }
                        } else {
                            cout << "Cannot renew this order (status: " << order.getStatus() << ")\n";
                            found = true;
                        }
                        break;
                    }
                }
                
                if (!found) cout << "Order not found!\n";
            } else if (subChoice == 4) { // Xem tất cả orders
                cout << "\n--- All Orders ---\n";
                if (orders.empty()) {
                    cout << "No orders found.\n";
                } else {
                    for (const auto &order : orders) {
                        order.displayOrderInfo();
                        cout << "---\n";
                    }
                }
            } else if (subChoice == 5) { // Xem đơn mượn đang hoạt động
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
            } else if (subChoice == 6) { // Xem đơn quá hạn
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
            }
            break;
        }
        
        case 3: { // Quản lý yêu cầu sách
            cout << "\n--- Manage Requests ---\n";
            cout << "1. View pending book requests\n";
            cout << "2. Approve book request\n";
            cout << "3. Reject book request\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Xem yêu cầu đang chờ
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
            } else if (subChoice == 2) { // Duyệt yêu cầu
                string requestId = prompt("Enter Request ID to approve: ");
                bool found = false;
                
                for (auto &request : bookRequests) {
                    if (request.getRequestId() == requestId) {
                        string notes = prompt("Approval notes (optional): ");
                        request.approveRequest(getCurrentDate(), notes);
                        autoSave(library, userService);
                        cout << "Request approved!\n";
                        found = true;
                        break;
                    }
                }
                
                if (!found) cout << "Request not found!\n";
            } else if (subChoice == 3) { // Từ chối yêu cầu
                string requestId = prompt("Enter Request ID to reject: ");
                bool found = false;
                
                for (auto &request : bookRequests) {
                    if (request.getRequestId() == requestId) {
                        string notes = prompt("Rejection reason: ");
                        request.rejectRequest(notes);
                        autoSave(library, userService);
                        cout << "Request rejected!\n";
                        found = true;
                        break;
                    }
                }
                
                if (!found) cout << "Request not found!\n";
            }
            break;
        }
        
        case 4: { // Báo cáo
            cout << "\n--- Generate Reports ---\n";
            cout << "1. Borrowing report\n";
            cout << "2. Inventory report\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) {
                ReportGenerator::borrowingTrends(orders);
            } else if (subChoice == 2) {
                ReportGenerator::inventory(library.getAllBooks());
            }
            break;
        }
        
        case 5: { // Truy cập chức năng User
            readerMenu(currentUser, library, userService);
            break;
        }
        
        case 6: { // Xem hồ sơ
            currentUser->displayUserInfo();
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

// Menu dành cho Admin (quản trị viên)
void adminMenu(User* currentUser, Library &library, UserService &userService) {
    bool running = true;
    while (running) {
        cout << "\n========== ADMIN MENU (" << currentUser->getName() << ") ==========\n";
        cout << "1. User Management\n";
        cout << "2. System Reports\n";
        cout << "3. Data Management\n";
        cout << "4. Access Librarian Features\n";
        cout << "5. Access User Features\n";
        cout << "6. My Profile\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: { // Quản lý người dùng
            cout << "\n--- User Management ---\n";
            cout << "1. Create Librarian account\n";
            cout << "2. View all users\n";
            cout << "3. View users by role\n";
            cout << "4. Deactivate user\n";
            cout << "5. Activate user\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Tạo tài khoản Librarian
                string name = prompt("Name: ");
                string email = prompt("Email: ");
                string password = prompt("Password: ");
                string phone = prompt("Phone Number: ");
                
                if (userService.registerUser(name, email, password, phone, Role::Librarian)) {
                    autoSave(library, userService);
                    cout << "Librarian account created successfully!\n";
                } else {
                    cout << "Failed to create account. Email may already exist.\n";
                }
            } else if (subChoice == 2) { // Xem tất cả người dùng
                userService.displayAllUsers();
            } else if (subChoice == 3) { // Xem người dùng theo vai trò
                cout << "Select role: 1=User, 2=Librarian, 3=Admin: ";
                int roleChoice;
                cin >> roleChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                Role role = (roleChoice == 1) ? Role::User : 
                           (roleChoice == 2) ? Role::Librarian : Role::Admin;
                
                auto users = userService.getUsersByRole(role);
                cout << "\n--- Users with role " << roleChoice << " ---\n";
                for (auto userPtr : users) {
                    userPtr->displayUserInfo();
                    cout << "---\n";
                }
            } else if (subChoice == 4) { // Vô hiệu hóa người dùng
                string email = prompt("User email to deactivate: ");
                User* user = userService.findUserByEmail(email);
                
                if (user) {
                    if (userService.deactivateUser(user->getId())) {
                        autoSave(library, userService);
                        cout << "User deactivated successfully!\n";
                    } else {
                        cout << "Failed to deactivate user.\n";
                    }
                } else {
                    cout << "User not found!\n";
                }
            } else if (subChoice == 5) { // Kích hoạt người dùng
                string email = prompt("User email to activate: ");
                User* user = userService.findUserByEmail(email);
                
                if (user) {
                    if (userService.activateUser(user->getId())) {
                        autoSave(library, userService);
                        cout << "User activated successfully!\n";
                    } else {
                        cout << "Failed to activate user.\n";
                    }
                } else {
                    cout << "User not found!\n";
                }
            }
            break;
        }
        
        case 2: { // System Reports
            cout << "\n--- System Reports ---\n";
            cout << "1. General statistics\n";
            cout << "2. User list report\n";
            cout << "3. Most borrowed books\n";
            cout << "4. Most active users\n";
            cout << "5. Popular categories\n";
            cout << "6. Overdue books report\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Thống kê tổng quan
                auto users = userService.getAllUsers();
                vector<User> userVector;
                for (auto userPtr : users) {
                    userVector.push_back(*userPtr);
                }
                ReportGenerator::generalStats(userVector, library.getAllBooks(), orders);
            } else if (subChoice == 2) { // Báo cáo danh sách người dùng
                auto users = userService.getAllUsers();
                vector<User> userVector;
                for (auto userPtr : users) {
                    userVector.push_back(*userPtr);
                }
                ReportGenerator::userList(userVector);
            } else if (subChoice == 3) { // Sách được mượn nhiều nhất
                ReportGenerator::mostBorrowedBooks(library.getAllBooks(), orders, 10);
            } else if (subChoice == 4) { // Người dùng tích cực nhất
                auto users = userService.getAllUsers();
                vector<User> userVector;
                for (auto userPtr : users) {
                    userVector.push_back(*userPtr);
                }
                ReportGenerator::mostActiveUsers(userVector, orders, 10);
            } else if (subChoice == 5) { // Danh mục phổ biến
                ReportGenerator::popularCategories(library.getAllBooks(), orders);
            } else if (subChoice == 6) { // Báo cáo sách quá hạn
                auto users = userService.getAllUsers();
                vector<User> userVector;
                for (auto userPtr : users) {
                    userVector.push_back(*userPtr);
                }
                ReportGenerator::overdueBooks(orders, library.getAllBooks(), userVector);
            }
            break;
        }
        
        case 3: { // Data Management
            cout << "\n--- Data Management ---\n";
            cout << "1. Backup system\n";
            cout << "2. Restore system\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            
            int subChoice;
            cin >> subChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (subChoice == 1) { // Backup hệ thống
                string filename = prompt("Backup filename (default: backup.txt): ");
                if (filename.empty()) filename = "backup.txt";
                
                if (library.saveToFile(filename + "_books") && 
                    userService.saveToFile(filename + "_users")) {
                    cout << "System backed up successfully!\n";
                    cout << "Files: " << filename << "_books, " << filename << "_users\n";
                } else {
                    cout << "Backup failed!\n";
                }
            } else if (subChoice == 2) { // Restore hệ thống
                string filename = prompt("Backup filename to restore: ");
                
                if (library.loadFromFile(filename + "_books") && 
                    userService.loadFromFile(filename + "_users")) {
                    cout << "System restored successfully!\n";
                } else {
                    cout << "Restore failed! Files may not exist.\n";
                }
            }
            break;
        }
        
        case 4: { // Truy cập chức năng Librarian
            librarianMenu(currentUser, library, userService);
            break;
        }
        
        case 5: { // Truy cập chức năng User
            readerMenu(currentUser, library, userService);
            break;
        }
        
        case 6: { // Xem hồ sơ
            currentUser->displayUserInfo();
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

// Hàm main - khởi tạo hệ thống và vòng lặp chính
int main() {
    try {
        // Khởi tạo đối tượng quản lý
        UserService userService;
        Library library;
        
        // Load dữ liệu từ file (nếu có)
        
        bool dataLoaded = userService.loadFromFile("database/users.txt");
        library.loadFromFile("database/books.txt");
        loadOrders();
        loadReservations();
        loadBookRequests();
        
    
    // Nếu chưa có dữ liệu, tạo sample accounts
    if (!dataLoaded) {
        userService.registerUser("Nguyen Van Admin", "admin@library.com", "admin123",
                                "0901234567", Role::Admin);
        
        userService.registerUser("Tran Thi Thu", "librarian1@library.com", "lib123",
                                "0912345678", Role::Librarian);
        
        userService.registerUser("Le Van Binh", "librarian2@library.com", "lib123",
                                "0923456789", Role::Librarian);
        
        userService.registerUser("Pham Minh Hieu", "reader1@library.com", "read123",
                                "0934567890", Role::User);
        
        userService.registerUser("Hoang Thi Lan", "reader2@library.com", "read123",
                                "0945678901", Role::User);
        
        userService.registerUser("Vo Quoc Bao", "reader3@library.com", "read123",
                                "0956789012", Role::User);
        
        // Lưu sample users ngay sau khi tạo
        autoSave(library, userService);
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
                    cout << "Login successful! Welcome, " << currentUser->getName() << "!\n";
                } else {
                    cout << "Login failed! Invalid email or password.\n";
                }
                break;
            }
            
            case 2: { // Đăng ký (chỉ Reader)
                string name = prompt("Name: ");
                string email = prompt("Email: ");
                string password = prompt("Password: ");
                string phone = prompt("Phone Number: ");
                
                if (userService.registerUser(name, email, password, phone)) {
                    autoSave(library, userService);
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
    userService.saveToFile("database/users.txt");
    library.saveToFile("database/books.txt");
    
    cout << "\n========================================\n";
    cout << "  Thank you for using our system!\n";
    cout << "  Goodbye!\n";
    cout << "========================================\n";
    
    } catch (const std::invalid_argument& e) {
        cerr << "\n[ERROR] Invalid argument: " << e.what() << endl;
        cerr << "This usually means corrupted data in text files." << endl;
        return 1;
    } catch (const std::exception& e) {
        cerr << "\n[ERROR] Exception: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}


