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
	     int rating,
	     string review,
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
	  rating(rating),
	  review(std::move(review)),
	  renewalPeriod(renewalPeriod),
	  bookCondition(std::move(bookCondition)) {}

const string &Order::getOrderId() const { return orderId; }
const string &Order::getUserId() const { return userId; }
const string &Order::getBookId() const { return bookId; }
const string &Order::getStatus() const { return status; }
int Order::getRating() const { return rating; }
const string &Order::getReview() const { return review; }
const string &Order::getBorrowDate() const { return borrowDate; }
const string &Order::getIssueDate() const { return issueDate; }
const string &Order::getDueDate() const { return dueDate; }
const string &Order::getReturnDate() const { return returnDate; }
int Order::getRenewalPeriod() const { return renewalPeriod; }
const string &Order::getBookCondition() const { return bookCondition; }

void Order::setStatus(const string &value) { status = value; }
void Order::setRating(int value) { rating = value; }
void Order::setReview(const string &value) { review = value; }
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

void Order::addReview(int ratingValue, const string &reviewText) {
	rating = ratingValue;
	review = reviewText;
	cout << "Review added to order " << orderId << " with rating " << rating 
	     << ": " << review << endl;
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
	if (rating > 0) {
		cout << "Rating: " << rating << "/5 stars\n";
		cout << "Review: " << review << endl;
	}
	cout << "Renewal Count: " << renewalPeriod << endl;
}

bool Order::isOverdue() const {
	// Simple check - trong thực tế nên so sánh với ngày hiện tại
	return status == "OVERDUE";
}
