#include "ReportGenerator.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <sstream>
#include <ctime>

using namespace std;

// ============= HELPER FUNCTIONS =============

string ReportGenerator::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "/"
       << (1900 + ltm->tm_year);
    return ss.str();
}

int ReportGenerator::daysBetweenDates(const string& date1, const string& date2) {
    int day1, month1, year1, day2, month2, year2;
    char sep;
    stringstream ss1(date1), ss2(date2);
    ss1 >> day1 >> sep >> month1 >> sep >> year1;
    ss2 >> day2 >> sep >> month2 >> sep >> year2;
    
    struct tm tm1 = {0}, tm2 = {0};
    tm1.tm_mday = day1; tm1.tm_mon = month1 - 1; tm1.tm_year = year1 - 1900;
    tm2.tm_mday = day2; tm2.tm_mon = month2 - 1; tm2.tm_year = year2 - 1900;
    
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    
    return (int)difftime(time2, time1) / (60 * 60 * 24);
}

bool ReportGenerator::isOverdue(const Order& order) {
    if (!order.getReturnDate().empty()) return false;
    
    string today = getCurrentDate();
    int daysOverdue = daysBetweenDates(order.getDueDate(), today);
    return daysOverdue > 0;
}

// ============= BASIC REPORTS =============

void ReportGenerator::generalStats(const vector<User>& users, 
                                   const vector<Book>& books,
                                   const vector<Order>& orders) {
    int totalQty = 0;
    int availableQty = 0;
    int borrowedCount = 0;
    int overdueCount = 0;
    
    for (const auto& b : books) {
        totalQty += b.getTotalQuantity();
    }
    
    for (const auto& o : orders) {
        if (o.getReturnDate().empty()) {
            borrowedCount++;
            if (isOverdue(o)) overdueCount++;
        }
    }
    
    availableQty = totalQty - borrowedCount;
    
    cout << "\n========================================\n";
    cout << "   GENERAL SYSTEM STATISTICS\n";
    cout << "========================================\n";
    cout << "Total Users:              " << users.size() << "\n";
    cout << "Total Book Titles:        " << books.size() << "\n";
    cout << "Total Book Copies:        " << totalQty << "\n";
    cout << "Available Books:          " << availableQty << "\n";
    cout << "Currently Borrowed:       " << borrowedCount << "\n";
    cout << "Overdue Books:            " << overdueCount << "\n";
    cout << "Total Borrowing Records:  " << orders.size() << "\n";
    cout << "========================================\n";
}

void ReportGenerator::inventory(const vector<Book>& books) {
    cout << "\n========================================\n";
    cout << "   BOOK INVENTORY REPORT\n";
    cout << "========================================\n";
    cout << left << setw(6) << "ID" << setw(38) << "Title" << setw(12) << "Quantity" << "Status\n";
    cout << "----------------------------------------\n";
    
    for (const auto& b : books) {
        string status;
        switch (b.getStatus()) {
            case BookStatus::Available: status = "Available"; break;
            case BookStatus::Borrowed: status = "Borrowed"; break;
            case BookStatus::Damaged: status = "Damaged"; break;
            case BookStatus::Lost: status = "Lost"; break;
            default: status = "Unknown"; break;
        }
        
        cout << left << setw(6) << b.getBookId() 
             << setw(38) << b.getTitle().substr(0, 37)
             << setw(12) << b.getTotalQuantity()
             << status << "\n";
    }
    
    cout << "========================================\n";
}

void ReportGenerator::userList(const vector<User>& users) {
    cout << "\n========================================\n";
    cout << "   USER LIST REPORT\n";
    cout << "========================================\n";
    cout << left << setw(6) << "ID" << setw(38) << "Full Name" << "Role\n";
    cout << "----------------------------------------\n";
    
    for (const auto& u : users) {
        string roleStr;
        switch (u.getRole()) {
            case Role::Reader: roleStr = "Reader"; break;
            case Role::Librarian: roleStr = "Librarian"; break;
            case Role::Admin: roleStr = "Administrator"; break;
        }
        
        cout << left << setw(6) << u.getId()
             << setw(38) << u.getName().substr(0, 37)
             << roleStr << "\n";
    }
    
    cout << "========================================\n";
}

// ============= BORROWING TRENDS =============

void ReportGenerator::borrowingTrends(const vector<Order>& orders) {
    map<string, int> monthlyCount;
    
    for (const auto& order : orders) {
        string date = order.getBorrowDate();
        if (date.length() >= 7) {
            string monthYear = date.substr(3, 2) + "/" + date.substr(6, 4); // MM/YYYY
            monthlyCount[monthYear]++;
        }
    }
    
    cout << "\n========================================\n";
    cout << "   MONTHLY BORROWING TRENDS\n";
    cout << "========================================\n";
    cout << left << setw(20) << "Month/Year" << "Borrowing Count\n";
    cout << "----------------------------------------\n";
    
    for (const auto& pair : monthlyCount) {
        cout << left << setw(20) << pair.first << pair.second << "\n";
    }
    
    cout << "========================================\n";
    cout << "Total active months: " << monthlyCount.size() << "\n";
}

// ============= POPULAR CATEGORIES =============

void ReportGenerator::popularCategories(const vector<Book>& books, const vector<Order>& orders) {
    map<string, int> categoryCount;
    
    for (const auto& order : orders) {
        for (const auto& book : books) {
            if (book.getBookId() == order.getBookId()) {
                for (const auto& cat : book.getCategories()) {
                    categoryCount[cat]++;
                }
                break;
            }
        }
    }
    
    vector<pair<string, int>> sortedCategories(categoryCount.begin(), categoryCount.end());
    sort(sortedCategories.begin(), sortedCategories.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    int total = 0;
    for (const auto& pair : sortedCategories) total += pair.second;
    
    cout << "\n========================================\n";
    cout << "   MOST POPULAR CATEGORIES\n";
    cout << "========================================\n";
    cout << left << setw(28) << "Category" << setw(12) << "Borrowed" << "Percentage\n";
    cout << "----------------------------------------\n";
    
    for (const auto& pair : sortedCategories) {
        double percentage = total > 0 ? (pair.second * 100.0 / total) : 0;
        cout << left << setw(28) << pair.first 
             << setw(12) << pair.second
             << fixed << setprecision(2) << percentage << "%\n";
    }
    
    cout << "========================================\n";
    cout << "Total borrowings: " << total << "\n";
}

// ============= BOOK BORROWING STATISTICS =============

void ReportGenerator::mostBorrowedBooks(const vector<Book>& books, 
                                       const vector<Order>& orders, int top) {
    map<string, int> borrowCount;
    map<string, string> bookTitles;
    
    for (const auto& order : orders) {
        borrowCount[order.getBookId()]++;
    }
    
    for (const auto& book : books) {
        bookTitles[book.getBookId()] = book.getTitle();
    }
    
    vector<pair<string, int>> sortedBooks(borrowCount.begin(), borrowCount.end());
    sort(sortedBooks.begin(), sortedBooks.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    cout << "\n========================================\n";
    cout << "   TOP " << top << " MOST BORROWED BOOKS\n";
    cout << "========================================\n";
    cout << left << setw(8) << "Rank" << setw(40) << "Book Title" << "Times Borrowed\n";
    cout << "----------------------------------------\n";
    
    int count = 0;
    for (const auto& pair : sortedBooks) {
        if (++count > top) break;
        
        string title = bookTitles[pair.first];
        cout << left << setw(8) << ("#" + to_string(count))
             << setw(40) << title.substr(0, 39)
             << pair.second << "\n";
    }
    
    cout << "========================================\n";
}

void ReportGenerator::currentlyBorrowedBooks(const vector<Order>& orders,
                                            const vector<Book>& books) {
    cout << "\n========================================\n";
    cout << "   CURRENTLY BORROWED BOOKS\n";
    cout << "========================================\n";
    cout << left << setw(6) << "ID" << setw(38) << "Title" << setw(12) << "Due Date" << "Status\n";
    cout << "----------------------------------------\n";
    
    int count = 0;
    for (const auto& order : orders) {
        if (order.getReturnDate().empty()) {
            string title = "N/A";
            for (const auto& book : books) {
                if (book.getBookId() == order.getBookId()) {
                    title = book.getTitle();
                    break;
                }
            }
            
            string status = isOverdue(order) ? "OVERDUE" : "On loan";
            
            cout << left << setw(6) << order.getBookId()
                 << setw(38) << title.substr(0, 37)
                 << setw(12) << order.getDueDate()
                 << status << "\n";
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No books currently borrowed.\n";
    }
    
    cout << "========================================\n";
    cout << "Total: " << count << " books\n";
}

// ============= BOOK RETURNING STATISTICS =============

void ReportGenerator::returningStatistics(const vector<Order>& orders) {
    int totalReturned = 0;
    int onTimeReturns = 0;
    int lateReturns = 0;
    int totalLateDays = 0;
    
    for (const auto& order : orders) {
        if (!order.getReturnDate().empty()) {
            totalReturned++;
            int daysLate = daysBetweenDates(order.getDueDate(), order.getReturnDate());
            
            if (daysLate > 0) {
                lateReturns++;
                totalLateDays += daysLate;
            } else {
                onTimeReturns++;
            }
        }
    }
    
    double onTimePercentage = totalReturned > 0 ? (onTimeReturns * 100.0 / totalReturned) : 0;
    double latePercentage = totalReturned > 0 ? (lateReturns * 100.0 / totalReturned) : 0;
    double avgLateDays = lateReturns > 0 ? (totalLateDays * 1.0 / lateReturns) : 0;
    
    cout << "\n========================================\n";
    cout << "   BOOK RETURN STATISTICS\n";
    cout << "========================================\n";
    cout << "Total books returned:     " << totalReturned << "\n";
    cout << "Returned on time:         " << onTimeReturns << "\n";
    cout << "Returned late:            " << lateReturns << "\n";
    cout << "On-time return rate:      " << fixed << setprecision(2) << onTimePercentage << "%\n";
    cout << "Late return rate:         " << fixed << setprecision(2) << latePercentage << "%\n";
    cout << "Average days late:        " << fixed << setprecision(1) << avgLateDays << "\n";
    cout << "========================================\n";
}

void ReportGenerator::overdueBooks(const vector<Order>& orders,
                                  const vector<Book>& books,
                                  const vector<User>& users) {
    cout << "\n========================================\n";
    cout << "   OVERDUE BOOKS REPORT\n";
    cout << "========================================\n";
    cout << left << setw(8) << "User" << setw(30) << "Book Title" << setw(12) << "Due Date" 
         << setw(12) << "Days Late" << "Fine (VND)\n";
    cout << "----------------------------------------\n";
    
    int count = 0;
    string today = getCurrentDate();
    
    for (const auto& order : orders) {
        if (order.getReturnDate().empty() && isOverdue(order)) {
            int daysOverdue = daysBetweenDates(order.getDueDate(), today);
            int fine = daysOverdue * 5000;
            
            string title = "N/A";
            for (const auto& book : books) {
                if (book.getBookId() == order.getBookId()) {
                    title = book.getTitle();
                    break;
                }
            }
            
            cout << left << setw(8) << order.getUserId()
                 << setw(30) << title.substr(0, 29)
                 << setw(12) << order.getDueDate()
                 << setw(12) << daysOverdue
                 << fine << "\n";
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No overdue books.\n";
    }
    
    cout << "========================================\n";
    cout << "Total overdue books: " << count << "\n";
}

// ============= USER ACTIVITIES =============

void ReportGenerator::mostActiveUsers(const vector<User>& users, 
                                     const vector<Order>& orders, int top) {
    map<string, int> userBorrowCount;
    map<string, string> userNames;
    
    for (const auto& order : orders) {
        userBorrowCount[order.getUserId()]++;
    }
    
    for (const auto& user : users) {
        stringstream ss;
        ss << user.getId();
        userNames[ss.str()] = user.getName();
    }
    
    vector<pair<string, int>> sortedUsers(userBorrowCount.begin(), userBorrowCount.end());
    sort(sortedUsers.begin(), sortedUsers.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    cout << "\n========================================\n";
    cout << "   TOP " << top << " MOST ACTIVE USERS\n";
    cout << "========================================\n";
    cout << left << setw(8) << "Rank" << setw(40) << "Full Name" << "Times Borrowed\n";
    cout << "----------------------------------------\n";
    
    int count = 0;
    for (const auto& pair : sortedUsers) {
        if (++count > top) break;
        
        string name = userNames[pair.first];
        cout << left << setw(8) << ("#" + to_string(count))
             << setw(40) << name.substr(0, 39)
             << pair.second << "\n";
    }
    
    cout << "========================================\n";
}

void ReportGenerator::userActivityReport(const User& user) {
    cout << "\n========================================\n";
    cout << "   USER ACTIVITY REPORT\n";
    cout << "========================================\n";
    cout << "User ID: " << user.getId() << "\n";
    cout << "Name:    " << user.getName() << "\n";
    cout << "----------------------------------------\n";
    cout << "No activity tracking available.\n";
    cout << "========================================\n";
}
