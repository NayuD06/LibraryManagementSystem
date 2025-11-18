#include "BorrowService.h"

#include <algorithm>
#include <iostream>

using namespace std;

BorrowService::BorrowService(BookService &service) 
	: bookService(service), orderCounter(0) {}

string BorrowService::generateOrderId() {
	orderCounter++;
	return "ORD-" + to_string(orderCounter);
}

Order *BorrowService::findOrder(const string &orderId) {
	for (auto &order : orders) {
		if (order.getOrderId() == orderId) {
			return &order;
		}
	}
	return nullptr;
}

const Order *BorrowService::findOrder(const string &orderId) const {
	for (const auto &order : orders) {
		if (order.getOrderId() == orderId) {
			return &order;
		}
	}
	return nullptr;
}

Order BorrowService::borrowBook(const string &userId,
								 const string &bookId,
								 const string &borrowDate,
								 const string &dueDate) {
	// Check if book exists and is available
	auto bookOpt = bookService.getBook(bookId);
	if (!bookOpt.has_value()) {
		throw runtime_error("Book not found");
	}
	
	auto book = bookOpt.value();
	if (book.getStatus() != "AVAILABLE") {
		throw runtime_error("Book is not available");
	}

	// Update book status to BORROWED
	book.setStatus("BORROWED");
	bookService.updateBook(bookId, book);

	// Create new order with status ISSUED
	Order newOrder(generateOrderId(),
				   userId,
				   bookId,
				   "ISSUED",
				   borrowDate,
				   borrowDate,  // issueDate same as borrowDate
				   dueDate,
				   "",          // returnDate empty
				   0,           // rating
				   "",          // review
				   0,           // renewalPeriod
				   "GOOD");     // bookCondition

	orders.push_back(newOrder);
	newOrder.createOrder();
	return newOrder;
}

bool BorrowService::returnBook(const string &orderId,
								const string &returnDate,
								const string &bookCondition) {
	Order *order = findOrder(orderId);
	if (!order) {
		return false;
	}

	// Update order
	order->setReturnDate(returnDate);
	order->setBookCondition(bookCondition);
	order->updateStatus("RETURNED");

	// Update book status
	auto bookOpt = bookService.getBook(order->getBookId());
	if (bookOpt.has_value()) {
		auto book = bookOpt.value();
		
		// Update book condition if damaged or lost
		if (bookCondition == "DAMAGED") {
			book.setCondition("DAMAGED");
		} else if (bookCondition == "LOST") {
			book.setCondition("LOST");
		}
		
		// Set book status back to AVAILABLE if in good condition
		if (bookCondition == "GOOD") {
			book.setStatus("AVAILABLE");
		}
		
		bookService.updateBook(order->getBookId(), book);
	}

	return true;
}

bool BorrowService::renewOrder(const string &orderId) {
	Order *order = findOrder(orderId);
	if (!order) {
		return false;
	}

	if (order->getStatus() != "ISSUED") {
		return false;
	}

	// Renew the order
	order->renewOrder();
	
	// Update due date (add 14 days - simplified, should use actual date calculation)
	// In real implementation, parse dueDate and add 14 days
	cout << "Order renewed. New due date should be calculated.\n";
	
	return true;
}

Order BorrowService::reserveBook(const string &userId,
								  const string &bookId,
								  const string &reservationDate) {
	// Create reservation order with status RESERVED
	Order reservation(generateOrderId(),
					  userId,
					  bookId,
					  "RESERVED",
					  reservationDate,
					  "",  // issueDate empty for reservation
					  "",  // dueDate empty for reservation
					  "",  // returnDate empty
					  0,   // rating
					  "",  // review
					  0,   // renewalPeriod
					  "GOOD");

	orders.push_back(reservation);
	cout << "Book reserved successfully. Order ID: " << reservation.getOrderId() << endl;
	return reservation;
}

bool BorrowService::cancelReservation(const string &orderId, const string &userId) {
	Order *order = findOrder(orderId);
	if (!order) {
		return false;
	}

	// Check if this order belongs to the user
	if (order->getUserId() != userId) {
		return false;
	}

	// Check if it's a reservation
	if (order->getStatus() != "RESERVED") {
		return false;
	}

	// Cancel reservation by updating status
	order->updateStatus("CANCELLED");
	return true;
}

vector<Order> BorrowService::getReservationsByUser(const string &userId) const {
	vector<Order> userReservations;
	for (const auto &order : orders) {
		if (order.getUserId() == userId && order.getStatus() == "RESERVED") {
			userReservations.push_back(order);
		}
	}
	return userReservations;
}

bool BorrowService::addReviewToOrder(const string &orderId,
									  int rating,
									  const string &review) {
	Order *order = findOrder(orderId);
	if (!order) {
		return false;
	}

	// Can only review returned orders
	if (order->getStatus() != "RETURNED") {
		return false;
	}

	order->addReview(rating, review);
	return true;
}

vector<Order> BorrowService::getBorrowHistory(const string &userId) const {
	vector<Order> history;
	for (const auto &order : orders) {
		if (order.getUserId() == userId) {
			history.push_back(order);
		}
	}
	return history;
}

vector<Order> BorrowService::getActiveOrders(const string &userId) const {
	vector<Order> activeOrders;
	for (const auto &order : orders) {
		if (order.getUserId() == userId && order.getStatus() == "ISSUED") {
			activeOrders.push_back(order);
		}
	}
	return activeOrders;
}

vector<Order> BorrowService::getOverdueOrders() const {
	vector<Order> overdueOrders;
	for (const auto &order : orders) {
		if (order.isOverdue()) {
			overdueOrders.push_back(order);
		}
	}
	return overdueOrders;
}

const vector<Order> &BorrowService::getAllOrders() const {
	return orders;
}

