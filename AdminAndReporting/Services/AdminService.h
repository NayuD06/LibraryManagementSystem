#pragma once
#include <vector>
#include <string>

struct User {
    int id;
    std::string name;
    std::string role;
};

struct Book {
    int id;
    std::string title;
    std::string author;
    int quantity;
};

class AdminService {
private:
    std::vector<User> users;
    std::vector<Book> books;

public:
    // User CRUD
    void addUser(int id, const std::string& name, const std::string& role);
    void updateUser(int id, const std::string& newName);
    void deleteUser(int id);

    // Book CRUD
    void addBook(int id, const std::string& title, const std::string& author, int quantity);
    void updateBook(int id, const std::string& newTitle, int newQty);
    void deleteBook(int id);

    // Backup / Restore
    bool backupSystem(const std::string& filepath);
    bool restoreSystem(const std::string& filepath);

    // getter
    const std::vector<User>& getUsers() const;
    const std::vector<Book>& getBooks() const;
};
