#include "ReportGenerator.h"
#include <iostream>

void ReportGenerator::generalStats(const std::vector<User>& users, const std::vector<Book>& books) {
    int totalQty = 0;
    for (const auto& b : books) totalQty += b.quantity;

    std::cout << "\n=== THONG KE HE THONG ===\n";
    std::cout << "Nguoi dung: " << users.size() << "\n";
    std::cout << "Dau sach: " << books.size() << "\n";
    std::cout << "Tong so luong sach: " << totalQty << "\n";
}

void ReportGenerator::inventory(const std::vector<Book>& books) {
    std::cout << "\n=== BAO CAO SACH ===\n";
    for (const auto& b : books)
        std::cout << b.id << " | " << b.title << " | SL: " << b.quantity << "\n";
}

void ReportGenerator::userList(const std::vector<User>& users) {
    std::cout << "\n=== DANH SACH NGUOI DUNG ===\n";
    for (const auto& u : users)
        std::cout << u.id << " | " << u.name << " | " << u.role << "\n";
}
