#include "AdminService.h"
#include <fstream>
#include <algorithm>
#include <iostream>

// ---------------- USER ----------------

void AdminService::addUser(int id, const std::string& name, const std::string& role) {
    users.push_back({ id, name, role });
}

void AdminService::updateUser(int id, const std::string& newName) {
    for (auto& u : users)
        if (u.id == id) { u.name = newName; return; }
}

void AdminService::deleteUser(int id) {
    users.erase(std::remove_if(users.begin(), users.end(),
        [&](const User& u){ return u.id == id; }),
        users.end());
}

// ---------------- BOOK ----------------

void AdminService::addBook(int id, const std::string& title, const std::string& author, int quantity) {
    books.push_back({ id, title, author, quantity });
}

void AdminService::updateBook(int id, const std::string& newTitle, int newQty) {
    for (auto& b : books)
        if (b.id == id) { b.title = newTitle; b.quantity = newQty; return; }
}

void AdminService::deleteBook(int id) {
    books.erase(std::remove_if(books.begin(), books.end(),
        [&](const Book& b){ return b.id == id; }),
        books.end());
}

// --------------- BACKUP ----------------

bool AdminService::backupSystem(const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out) return false;

    out << users.size() << "\n";
    for (const auto& u : users)
        out << u.id << "," << u.name << "," << u.role << "\n";

    out << books.size() << "\n";
    for (const auto& b : books)
        out << b.id << "," << b.title << "," << b.author << "," << b.quantity << "\n";

    return true;
}

bool AdminService::restoreSystem(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in) return false;

    // Chưa triển khai đầy đủ (tùy format bạn muốn parse)
    return true;
}

// --------------- GETTER ----------------

const std::vector<User>& AdminService::getUsers() const { return users; }
const std::vector<Book>& AdminService::getBooks() const { return books; }
