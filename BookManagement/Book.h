#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum class BookStatus { Available, Borrowed, Damaged, Lost };
enum class BookCondition { Excellent, Good, Fair, Poor, Damaged, Lost };

class Book {
private:
    string bookId;              // Unique book ID
    string title;
    string author;
    string publisher;
    int yearOfPublication;
    string ISBN;
    vector<string> categories;  // Can belong to multiple categories
    vector<string> keywords;
    int totalQuantity;          // Total number of copies
    int availableQuantity;      // Available copies
    int totalPages;
    string briefDescription;
    string detailedDescription;
    BookStatus status;
    BookCondition condition;
    int viewCount;              // Number of views
    double rentalPrice;
    double purchasePrice;

public:
    // Constructors
    Book();
    Book(const string& bookId, const string& title, const string& author, 
         const string& publisher, int year, const string& ISBN,
         const vector<string>& categories, const vector<string>& keywords,
         int quantity, int pages, const string& briefDesc, const string& detailedDesc,
         double rentalPrice, double purchasePrice);

    // Getters
    string getBookId() const;
    string getTitle() const;
    string getAuthor() const;
    string getPublisher() const;
    int getYearOfPublication() const;
    string getISBN() const;
    const vector<string>& getCategories() const;
    const vector<string>& getKeywords() const;
    int getTotalQuantity() const;
    int getAvailableQuantity() const;
    int getTotalPages() const;
    string getBriefDescription() const;
    string getDetailedDescription() const;
    BookStatus getStatus() const;
    BookCondition getCondition() const;
    int getViewCount() const;
    double getRentalPrice() const;
    double getPurchasePrice() const;

    // Setters for updating book information
    void setTitle(const string& newTitle);
    void setAuthor(const string& newAuthor);
    void setPublisher(const string& newPublisher);
    void setYearOfPublication(int year);
    void setISBN(const string& newISBN);
    void setCategories(const vector<string>& newCategories);
    void setKeywords(const vector<string>& newKeywords);
    void setTotalQuantity(int newQuantity);
    void setTotalPages(int pages);
    void setBriefDescription(const string& desc);
    void setDetailedDescription(const string& desc);
    void setCondition(BookCondition cond);
    void setRentalPrice(double price);
    void setPurchasePrice(double price);

    // Book operations
    void displayBookInfo() const;
    void displayDetailedInfo() const;
    void updateStatus();                    // Auto-update status based on availability
    bool isAvailable() const;
    void decreaseAvailableQuantity();       // When borrowed
    void increaseAvailableQuantity();       // When returned
    void incrementViewCount();              // Track popularity
    
    // Status management
    string statusToString() const;
    string conditionToString() const;
    static BookStatus stringToStatus(const string& statusStr);
    static BookCondition stringToCondition(const string& condStr);

    // Serialization for file storage
    string serialize() const;
    static Book deserialize(const string& line);
};

#endif