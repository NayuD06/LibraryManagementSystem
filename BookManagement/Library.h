#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class Library {
private:
    vector<Book> books;
    int nextBookId;

    // Helper function to generate unique book ID
    string generateBookId();

public:
    Library();

    // Book Management
    void addBook(const Book& book);
    void addNewBook(const string& title, const string& author, const string& publisher,
                   int year, const string& ISBN, const vector<string>& categories,
                   const vector<string>& keywords, int quantity, int pages,
                   const string& briefDesc, const string& detailedDesc,
                   double rentalPrice, double purchasePrice);
    
    bool updateBook(const string& bookId, const string& newTitle, const string& newAuthor,
                   const string& newPublisher, int newYear, const vector<string>& newCategories,
                   const vector<string>& newKeywords, int newQuantity, int newPages,
                   const string& newBriefDesc, const string& newDetailedDesc,
                   double newRentalPrice, double newPurchasePrice);
    
    bool removeBook(const string& bookId);
    
    // Book Status Tracking
    void trackBookStatus(const string& bookId) const;
    Book* findBookById(const string& bookId);
    const Book* findBookById(const string& bookId) const;
    Book* findBookByISBN(const string& ISBN);

    // Search and Retrieval
    vector<Book*> searchByTitle(const string& keyword);
    vector<Book*> searchByAuthor(const string& authorName);
    vector<Book*> searchByKeyword(const string& keyword);
    vector<Book*> filterByCategory(const string& category);
    vector<Book*> searchBooks(const string& query); // General search
    
    // View and Display
    void displayAllBooks() const;
    void displayBooksByCategory() const;
    void displayBooksByAuthor() const;
    vector<Book*> sortByPopularity(); // Sort by view count
    
    // Data Management
    const vector<Book>& getAllBooks() const;
    int getTotalBooks() const;
    int getAvailableBooks() const;
    
    // File operations
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
};

#endif

