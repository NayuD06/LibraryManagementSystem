#include "Order.h"

#include <iostream>
#include <ctime>

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
	// Tính số ngày quá hạn
	if (returnDate.empty() || dueDate.empty()) return 0;
	
	// Parse ngày trả và ngày hạn (format: YYYY-MM-DD)
	int retYear, retMonth, retDay;
	int dueYear, dueMonth, dueDay;
	
	sscanf(returnDate.c_str(), "%d-%d-%d", &retYear, &retMonth, &retDay);
	sscanf(dueDate.c_str(), "%d-%d-%d", &dueYear, &dueMonth, &dueDay);
	
	// Chuyển sang struct tm
	tm retTm = {0};
	retTm.tm_year = retYear - 1900;
	retTm.tm_mon = retMonth - 1;
	retTm.tm_mday = retDay;
	
	tm dueTm = {0};
	dueTm.tm_year = dueYear - 1900;
	dueTm.tm_mon = dueMonth - 1;
	dueTm.tm_mday = dueDay;
	
	// Tính chênh lệch (giây)
	time_t retTime = mktime(&retTm);
	time_t dueTime = mktime(&dueTm);
	
	double diffSeconds = difftime(retTime, dueTime);
	int diffDays = (int)(diffSeconds / (24 * 60 * 60));
	
	return (diffDays > 0) ? diffDays : 0;
}

double Order::calculateFine(double dailyFineRate, double bookPrice) const {
    double totalFine = 0.0;
    int overdueDays = calculateOverdueDays();
    if (overdueDays > 0) {
        totalFine += overdueDays * dailyFineRate;
    }
    if (bookCondition == "DAMAGED") {
        totalFine += bookPrice * 0.5;
    } else if (bookCondition == "LOST") {
        totalFine += bookPrice;
    }
    return totalFine;
}