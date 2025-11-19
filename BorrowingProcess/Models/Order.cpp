#include "Order.h"

#include <iostream>

using namespace std;

Order::Order(string orderId,
	     string userId,
	     string bookId,
	     string status,
	     string borrowDate,
	     string issueDate,
	     string dueDate,
	     string returnDate,
	     int renewalPeriod,
	     string bookCondition)
	: orderId(std::move(orderId)),
	  userId(std::move(userId)),
	  bookId(std::move(bookId)),
	  status(std::move(status)),
	  borrowDate(std::move(borrowDate)),
	  issueDate(std::move(issueDate)),
	  dueDate(std::move(dueDate)),
	  returnDate(std::move(returnDate)),
	  renewalPeriod(renewalPeriod),
	  bookCondition(std::move(bookCondition)) {}

const string &Order::getOrderId() const { return orderId; }
const string &Order::getUserId() const { return userId; }
const string &Order::getBookId() const { return bookId; }
const string &Order::getStatus() const { return status; }
const string &Order::getBorrowDate() const { return borrowDate; }
const string &Order::getIssueDate() const { return issueDate; }
const string &Order::getDueDate() const { return dueDate; }
const string &Order::getReturnDate() const { return returnDate; }
int Order::getRenewalPeriod() const { return renewalPeriod; }
const string &Order::getBookCondition() const { return bookCondition; }

void Order::setStatus(const string &value) { status = value; }
void Order::setIssueDate(const string &value) { issueDate = value; }
void Order::setDueDate(const string &value) { dueDate = value; }
void Order::setReturnDate(const string &value) { returnDate = value; }
void Order::setRenewalPeriod(int value) { renewalPeriod = value; }
void Order::setBookCondition(const string &value) { bookCondition = value; }

void Order::createOrder() {
	cout << "Creating order " << orderId << " for user " << userId 
	     << " to borrow book " << bookId << " on " << borrowDate << endl;
}

void Order::updateStatus(const string &newStatus) {
	status = newStatus;
	cout << "Order " << orderId << " status updated to: " << status << endl;
}

void Order::renewOrder() {
	renewalPeriod++;
	cout << "Order " << orderId << " renewed. Renewal count: " << renewalPeriod << endl;
}

void Order::displayOrderInfo() const {
	cout << "\n=== Order Information ===\n";
	cout << "Order ID: " << orderId << endl;
	cout << "User ID: " << userId << endl;
	cout << "Book ID: " << bookId << endl;
	cout << "Status: " << status << endl;
	cout << "Borrow Date: " << borrowDate << endl;
	if (!issueDate.empty()) {
		cout << "Issue Date: " << issueDate << endl;
	}
	if (!dueDate.empty()) {
		cout << "Due Date: " << dueDate << endl;
	}
	if (!returnDate.empty()) {
		cout << "Return Date: " << returnDate << endl;
		cout << "Book Condition: " << bookCondition << endl;
	}
	cout << "Renewal Count: " << renewalPeriod << endl;
}

bool Order::isOverdue() const {
	// Simple check - trong thực tế nên so sánh với ngày hiện tại
	return status == "OVERDUE";
}

int Order::calculateOverdueDays() const {
	// Tính số ngày quá hạn (simplified - giả định mỗi ngày cách nhau 1 đơn vị)
	if (returnDate.empty() || dueDate.empty()) return 0;
	
	// So sánh chuỗi ngày (format: YYYY-MM-DD)
	if (returnDate > dueDate) {
		// Simplified: giả định mỗi ngày = 1
		// Trong thực tế cần parse date và tính chênh lệch
		return 5; // Placeholder - trả về số ngày cố định để demo
	}
	return 0;
}

double Order::calculateFine(double dailyFineRate, double bookPrice) const {
	double totalFine = 0.0;
	
	// Phí phạt quá hạn
	int overdueDays = calculateOverdueDays();
	if (overdueDays > 0) {
		totalFine += overdueDays * dailyFineRate;
	}
	
	// Phí đền bù nếu sách hỏng/mất
	if (bookCondition == "DAMAGED") {
		totalFine += bookPrice * 0.5; // 50% giá sách nếu hỏng
	} else if (bookCondition == "LOST") {
		totalFine += bookPrice; // 100% giá sách nếu mất
	}
	
	return totalFine;
}
