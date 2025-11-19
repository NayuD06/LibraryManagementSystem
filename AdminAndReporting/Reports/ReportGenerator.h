#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <vector>
#include <string>
#include "../../UserManagement/User.h"
#include "../../BookManagement/Book.h"
#include "../../BorrowingProcess/Models/Order.h"

class ReportGenerator {
public:
    // Basic reports
    static void generalStats(const std::vector<User>& users, 
                            const std::vector<Book>& books,
                            const std::vector<Order>& orders);
    
    static void inventory(const std::vector<Book>& books);
    static void userList(const std::vector<User>& users);
    
    // Borrowing trends
    static void borrowingTrends(const std::vector<Order>& orders);
    
    // Popular categories
    static void popularCategories(const std::vector<Book>& books, 
                                  const std::vector<Order>& orders);
    
    // Book borrowing statistics
    static void mostBorrowedBooks(const std::vector<Book>& books, 
                                 const std::vector<Order>& orders, 
                                 int top = 10);
    
    static void currentlyBorrowedBooks(const std::vector<Order>& orders,
                                      const std::vector<Book>& books);
    
    // Book returning statistics
    static void returningStatistics(const std::vector<Order>& orders);
    static void overdueBooks(const std::vector<Order>& orders,
                            const std::vector<Book>& books,
                            const std::vector<User>& users);
    
    // User activities
    static void mostActiveUsers(const std::vector<User>& users, 
                               const std::vector<Order>& orders, 
                               int top = 10);
    
    static void userActivityReport(const User& user);

private:
    static bool isOverdue(const Order& order);
    static std::string getCurrentDate();
    static int daysBetweenDates(const std::string& date1, const std::string& date2);
};

#endif
