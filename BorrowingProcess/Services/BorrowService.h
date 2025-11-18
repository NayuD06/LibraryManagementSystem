// ===== BORROW SERVICE =====
// Service quản lý quá trình mượn/trả sách
// Theo Class Diagram và Functional Requirements
// Chức năng:
// - Mượn sách (borrowBook) - tạo Order mới với status ISSUED
// - Trả sách (returnBook) - cập nhật Order với returnDate và bookCondition
// - Gia hạn sách (renewOrder) - tăng renewalPeriod và cập nhật dueDate
// - Đặt trước sách (reserveBook) - tạo Order với status RESERVED
// - Xem lịch sử mượn của user
// - Thêm review cho Order

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "BookManagement/Services/BookService.h"
#include "BorrowingProcess/Models/Order.h"

class BorrowService {
private:
	std::vector<Order> orders;
	BookService &bookService;
	int orderCounter;

	Order *findOrder(const std::string &orderId);
	const Order *findOrder(const std::string &orderId) const;
	std::string generateOrderId();

public:
	explicit BorrowService(BookService &bookService);

	// Core borrowing operations
	Order borrowBook(const std::string &userId,
					 const std::string &bookId,
					 const std::string &borrowDate,
					 const std::string &dueDate);

	bool returnBook(const std::string &orderId,
					const std::string &returnDate,
					const std::string &bookCondition = "GOOD");

	bool renewOrder(const std::string &orderId);
	
	// Reservation operations  
	Order reserveBook(const std::string &userId,
					  const std::string &bookId,
					  const std::string &reservationDate);
					  
	bool cancelReservation(const std::string &orderId, const std::string &userId);
	std::vector<Order> getReservationsByUser(const std::string &userId) const;
	
	// Review operations
	bool addReviewToOrder(const std::string &orderId,
						  int rating,
						  const std::string &review);
	
	// History and queries
	std::vector<Order> getBorrowHistory(const std::string &userId) const;
	std::vector<Order> getActiveOrders(const std::string &userId) const;
	std::vector<Order> getOverdueOrders() const;
	const std::vector<Order> &getAllOrders() const;
};


