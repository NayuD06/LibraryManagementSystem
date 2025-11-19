#include "Book.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// Constructors
Book::Book() : totalQuantity(0), availableQuantity(0), totalPages(0), 
               viewCount(0), rentalPrice(0.0),
               status(BookStatus::Available), condition(BookCondition::Good) {}

Book::Book(const string& bookId, const string& title, const string& author,
           const string& publisher, int year, const string& ISBN,
           const vector<string>& categories, const vector<string>& keywords,
           int quantity, int pages, const string& briefDesc, const string& detailedDesc,
           double rentalPrice)
    : bookId(bookId), title(title), author(author), publisher(publisher),
      yearOfPublication(year), ISBN(ISBN), categories(categories), keywords(keywords),
      totalQuantity(quantity), availableQuantity(quantity), totalPages(pages),
      briefDescription(briefDesc), detailedDescription(detailedDesc),
      viewCount(0), rentalPrice(rentalPrice),
      status(BookStatus::Available), condition(BookCondition::Good) {
    // Cập nhật status dựa trên số lượng thực tế
    updateStatus();
}

// Getters
string Book::getBookId() const { return bookId; }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getPublisher() const { return publisher; }
int Book::getYearOfPublication() const { return yearOfPublication; }
string Book::getISBN() const { return ISBN; }
const vector<string>& Book::getCategories() const { return categories; }
const vector<string>& Book::getKeywords() const { return keywords; }
int Book::getTotalQuantity() const { return totalQuantity; }
int Book::getAvailableQuantity() const { return availableQuantity; }
int Book::getTotalPages() const { return totalPages; }
string Book::getBriefDescription() const { return briefDescription; }
string Book::getDetailedDescription() const { return detailedDescription; }
BookStatus Book::getStatus() const { return status; }
BookCondition Book::getCondition() const { return condition; }
int Book::getViewCount() const { return viewCount; }
double Book::getRentalPrice() const { return rentalPrice; }

// Setters
void Book::setTitle(const string& newTitle) { title = newTitle; }
void Book::setAuthor(const string& newAuthor) { author = newAuthor; }
void Book::setPublisher(const string& newPublisher) { publisher = newPublisher; }
void Book::setYearOfPublication(int year) { yearOfPublication = year; }
void Book::setISBN(const string& newISBN) { ISBN = newISBN; }
void Book::setCategories(const vector<string>& newCategories) { categories = newCategories; }
void Book::setKeywords(const vector<string>& newKeywords) { keywords = newKeywords; }
void Book::setTotalQuantity(int newQuantity) { 
    totalQuantity = newQuantity;
    updateStatus();
}
void Book::setTotalPages(int pages) { totalPages = pages; }
void Book::setBriefDescription(const string& desc) { briefDescription = desc; }
void Book::setDetailedDescription(const string& desc) { detailedDescription = desc; }
void Book::setCondition(BookCondition cond) { condition = cond; }
void Book::setRentalPrice(double price) { rentalPrice = price; }

// Display book information
void Book::displayBookInfo() const {
    cout << "\n----- Book Information -----\n";
    cout << "Book ID: " << bookId << "\n";
    cout << "Title: " << title << "\n";
    cout << "Author: " << author << "\n";
    cout << "Publisher: " << publisher << "\n";
    cout << "Year: " << yearOfPublication << "\n";
    cout << "ISBN: " << ISBN << "\n";
    
    cout << "Categories: ";
    for (size_t i = 0; i < categories.size(); ++i) {
        cout << categories[i];
        if (i < categories.size() - 1) cout << ", ";
    }
    cout << "\n";
    
    cout << "Available: " << availableQuantity << "/" << totalQuantity << "\n";
    cout << "Status: " << statusToString() << "\n";
    cout << "Condition: " << conditionToString() << "\n";
    cout << "Views: " << viewCount << "\n";
    cout << "Rental Price: $" << fixed << setprecision(2) << rentalPrice << "/day\n";
    cout << "Brief: " << briefDescription << "\n";
}

void Book::displayDetailedInfo() const {
    displayBookInfo();
    cout << "\n----- Detailed Information -----\n";
    cout << "Total Pages: " << totalPages << "\n";
    cout << "Keywords: ";
    for (size_t i = 0; i < keywords.size(); ++i) {
        cout << keywords[i];
        if (i < keywords.size() - 1) cout << ", ";
    }
    cout << "\n";
    cout << "Detailed Description:\n" << detailedDescription << "\n";
}

// Auto-update status based on availability and condition
void Book::updateStatus() {
    if (condition == BookCondition::Damaged) {
        status = BookStatus::Damaged;
    } else if (condition == BookCondition::Lost) {
        status = BookStatus::Lost;
    } else if (availableQuantity > 0) {
        status = BookStatus::Available;
    } else {
        status = BookStatus::Borrowed;
    }
}

bool Book::isAvailable() const {
    return availableQuantity > 0 && 
           status != BookStatus::Damaged && 
           status != BookStatus::Lost;
}

void Book::decreaseAvailableQuantity() {
    if (availableQuantity > 0) {
        availableQuantity--;
        updateStatus();
    }
}

void Book::increaseAvailableQuantity() {
    if (availableQuantity < totalQuantity) {
        availableQuantity++;
        updateStatus();
    }
}

void Book::incrementViewCount() {
    viewCount++;
}

// Status conversion
string Book::statusToString() const {
    switch (status) {
        case BookStatus::Available: return "Available";
        case BookStatus::Borrowed: return "Borrowed";
        case BookStatus::Damaged: return "Damaged";
        case BookStatus::Lost: return "Lost";
        default: return "Unknown";
    }
}

string Book::conditionToString() const {
    switch (condition) {
        case BookCondition::Excellent: return "Excellent";
        case BookCondition::Good: return "Good";
        case BookCondition::Fair: return "Fair";
        case BookCondition::Poor: return "Poor";
        case BookCondition::Damaged: return "Damaged";
        case BookCondition::Lost: return "Lost";
        default: return "Unknown";
    }
}

BookStatus Book::stringToStatus(const string& statusStr) {
    if (statusStr == "Available") return BookStatus::Available;
    if (statusStr == "Borrowed") return BookStatus::Borrowed;
    if (statusStr == "Damaged") return BookStatus::Damaged;
    if (statusStr == "Lost") return BookStatus::Lost;
    return BookStatus::Available;
}

BookCondition Book::stringToCondition(const string& condStr) {
    if (condStr == "Excellent") return BookCondition::Excellent;
    if (condStr == "Good") return BookCondition::Good;
    if (condStr == "Fair") return BookCondition::Fair;
    if (condStr == "Poor") return BookCondition::Poor;
    if (condStr == "Damaged") return BookCondition::Damaged;
    if (condStr == "Lost") return BookCondition::Lost;
    return BookCondition::Good;
}

// Serialization
string Book::serialize() const {
    stringstream ss;
    ss << bookId << "|" << title << "|" << author << "|" << publisher << "|"
       << yearOfPublication << "|" << ISBN << "|";
    
    // Serialize categories
    for (size_t i = 0; i < categories.size(); ++i) {
        ss << categories[i];
        if (i < categories.size() - 1) ss << ",";
    }
    ss << "|";
    
    // Serialize keywords
    for (size_t i = 0; i < keywords.size(); ++i) {
        ss << keywords[i];
        if (i < keywords.size() - 1) ss << ",";
    }
    ss << "|";
    
    ss << totalQuantity << "|" << availableQuantity << "|" << totalPages << "|"
       << briefDescription << "|" << detailedDescription << "|"
       << statusToString() << "|" << conditionToString() << "|"
       << viewCount << "|" << rentalPrice;
    
    return ss.str();
}

Book Book::deserialize(const string& line) {
    stringstream ss(line);
    string bookId, title, author, publisher, yearStr, ISBN, catStr, kwStr;
    string qtyStr, availStr, pagesStr, briefDesc, detailDesc, statusStr, condStr;
    string viewStr, rentalStr;
    
    getline(ss, bookId, '|');
    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, publisher, '|');
    getline(ss, yearStr, '|');
    getline(ss, ISBN, '|');
    getline(ss, catStr, '|');
    getline(ss, kwStr, '|');
    getline(ss, qtyStr, '|');
    getline(ss, availStr, '|');
    getline(ss, pagesStr, '|');
    getline(ss, briefDesc, '|');
    getline(ss, detailDesc, '|');
    getline(ss, statusStr, '|');
    getline(ss, condStr, '|');
    getline(ss, viewStr, '|');
    getline(ss, rentalStr, '|');
    
    // Parse categories
    vector<string> categories;
    if (!catStr.empty()) {
        stringstream catStream(catStr);
        string cat;
        while (getline(catStream, cat, ',')) {
            categories.push_back(cat);
        }
    }
    
    // Parse keywords
    vector<string> keywords;
    if (!kwStr.empty()) {
        stringstream kwStream(kwStr);
        string kw;
        while (getline(kwStream, kw, ',')) {
            keywords.push_back(kw);
        }
    }
    
    Book book(bookId, title, author, publisher, stoi(yearStr), ISBN,
              categories, keywords, stoi(qtyStr), stoi(pagesStr),
              briefDesc, detailDesc, stod(rentalStr));
    
    book.availableQuantity = stoi(availStr);
    book.status = stringToStatus(statusStr);
    book.condition = stringToCondition(condStr);
    book.viewCount = stoi(viewStr);
    
    return book;
}

