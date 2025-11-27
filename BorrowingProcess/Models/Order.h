#pragma once

#include <string>

class Order {
private:
	std::string orderId;
	std::string userId;
	std::string bookId;
	std::string status;
	std::string borrowDate;
	std::string issueDate;
	std::string dueDate;
	std::string returnDate;
	int renewalPeriod;
	std::string bookCondition;

public:
	Order() = default;
	Order(std::string orderId,
		  std::string userId,
		  std::string bookId,
		  std::string status,
		  std::string borrowDate,
		  std::string issueDate = "",
		  std::string dueDate = "",
		  std::string returnDate = "",
		  int renewalPeriod = 0,
		  std::string bookCondition = "GOOD");

	const std::string &getOrderId() const;
	const std::string &getUserId() const;
	const std::string &getBookId() const;
	const std::string &getStatus() const;
	const std::string &getBorrowDate() const;
	const std::string &getIssueDate() const;
	const std::string &getDueDate() const;
	const std::string &getReturnDate() const;
	int getRenewalPeriod() const;
	const std::string &getBookCondition() const;

	void setStatus(const std::string &value);
	void setIssueDate(const std::string &value);
	void setDueDate(const std::string &value);
	void setReturnDate(const std::string &value);
	void setRenewalPeriod(int value);
	void setBookCondition(const std::string &value);

	// Methods theo class diagram
	void createOrder();
	void updateStatus(const std::string &newStatus);
	void renewOrder();
	
	// Helper methods
	void displayOrderInfo() const;
	bool isOverdue() const;
	int calculateOverdueDays() const;
	double calculateFine(double dailyFineRate, double bookPrice) const;
};
