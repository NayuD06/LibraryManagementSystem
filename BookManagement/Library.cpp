#include "Library.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

Library::Library() : nextBookId(1) {}

string Library::generateBookId() {
    stringstream ss;
    ss << "B" << setfill('0') << setw(5) << nextBookId++;
    return ss.str();
}

// Book Management
void Library::addBook(const Book& book) {
    books.push_back(book);
    cout << "Book \"" << book.getTitle() << "\" added successfully.\n";
}

void Library::addNewBook(const string& title, const string& author, const string& publisher,
                        int year, const string& ISBN, const vector<string>& categories,
                        const vector<string>& keywords, int quantity, int pages,
                        const string& briefDesc, const string& detailedDesc,
                        double rentalPrice) {
    string bookId = generateBookId();
    Book newBook(bookId, title, author, publisher, year, ISBN, categories, keywords,
                quantity, pages, briefDesc, detailedDesc, rentalPrice);
    books.push_back(newBook);
    cout << "Book \"" << title << "\" added with ID: " << bookId << "\n";
}

bool Library::updateBook(const string& bookId, const string& newTitle, const string& newAuthor,
                        const string& newPublisher, int newYear, const vector<string>& newCategories,
                        const vector<string>& newKeywords, int newQuantity, int newPages,
                        const string& newBriefDesc, const string& newDetailedDesc,
                        double newRentalPrice) {
    for (Book& book : books) {
        if (book.getBookId() == bookId) {
            book.setTitle(newTitle);
            book.setAuthor(newAuthor);
            book.setPublisher(newPublisher);
            book.setYearOfPublication(newYear);
            book.setCategories(newCategories);
            book.setKeywords(newKeywords);
            book.setTotalQuantity(newQuantity);
            book.setTotalPages(newPages);
            book.setBriefDescription(newBriefDesc);
            book.setDetailedDescription(newDetailedDesc);
            book.setRentalPrice(newRentalPrice);
            cout << "Book with ID " << bookId << " updated successfully.\n";
            return true;
        }
    }
    cout << "Book with ID " << bookId << " not found.\n";
    return false;
}

bool Library::removeBook(const string& bookId) {
    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].getBookId() == bookId) {
            cout << "Removing book: \"" << books[i].getTitle() << "\"\n";
            books.erase(books.begin() + i);
            return true;
        }
    }
    cout << "Book with ID " << bookId << " not found.\n";
    return false;
}

// Book Status Tracking
void Library::trackBookStatus(const string& bookId) const {
    const Book* book = findBookById(bookId);
    if (book) {
        cout << "\n===== Book Status =====\n";
        cout << "Title: " << book->getTitle() << "\n";
        cout << "Available: " << book->getAvailableQuantity() 
             << " out of " << book->getTotalQuantity() << "\n";
        cout << "Status: " << book->statusToString() << "\n";
        cout << "Condition: " << book->conditionToString() << "\n";
    } else {
        cout << "Book with ID " << bookId << " not found.\n";
    }
}

Book* Library::findBookById(const string& bookId) {
    for (Book& book : books) {
        if (book.getBookId() == bookId) {
            return &book;
        }
    }
    return nullptr;
}

const Book* Library::findBookById(const string& bookId) const {
    for (const Book& book : books) {
        if (book.getBookId() == bookId) {
            return &book;
        }
    }
    return nullptr;
}

Book* Library::findBookByISBN(const string& ISBN) {
    for (Book& book : books) {
        if (book.getISBN() == ISBN) {
            return &book;
        }
    }
    return nullptr;
}

// Search and Retrieval
vector<Book*> Library::searchByTitle(const string& keyword) {
    vector<Book*> results;
    string lowerKeyword = keyword;
    transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    for (Book& book : books) {
        string lowerTitle = book.getTitle();
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        
        if (lowerTitle.find(lowerKeyword) != string::npos) {
            results.push_back(&book);
        }
    }
    return results;
}

vector<Book*> Library::searchByAuthor(const string& authorName) {
    vector<Book*> results;
    string lowerAuthor = authorName;
    transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
    
    for (Book& book : books) {
        string lowerBookAuthor = book.getAuthor();
        transform(lowerBookAuthor.begin(), lowerBookAuthor.end(), lowerBookAuthor.begin(), ::tolower);
        
        if (lowerBookAuthor.find(lowerAuthor) != string::npos) {
            results.push_back(&book);
        }
    }
    return results;
}

vector<Book*> Library::searchByKeyword(const string& keyword) {
    vector<Book*> results;
    string lowerKeyword = keyword;
    transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    for (Book& book : books) {
        for (const string& kw : book.getKeywords()) {
            string lowerKw = kw;
            transform(lowerKw.begin(), lowerKw.end(), lowerKw.begin(), ::tolower);
            
            if (lowerKw.find(lowerKeyword) != string::npos) {
                results.push_back(&book);
                break;
            }
        }
    }
    return results;
}

vector<Book*> Library::filterByCategory(const string& category) {
    vector<Book*> results;
    string lowerCategory = category;
    transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(), ::tolower);
    
    for (Book& book : books) {
        for (const string& cat : book.getCategories()) {
            string lowerCat = cat;
            transform(lowerCat.begin(), lowerCat.end(), lowerCat.begin(), ::tolower);
            
            if (lowerCat == lowerCategory || lowerCat.find(lowerCategory) != string::npos) {
                results.push_back(&book);
                break;
            }
        }
    }
    return results;
}

vector<Book*> Library::searchBooks(const string& query) {
    vector<Book*> results;
    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (Book& book : books) {
        string lowerTitle = book.getTitle();
        string lowerAuthor = book.getAuthor();
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
        
        bool found = false;
        
        // Search in title, author
        if (lowerTitle.find(lowerQuery) != string::npos || 
            lowerAuthor.find(lowerQuery) != string::npos) {
            found = true;
        }
        
        // Search in categories
        if (!found) {
            for (const string& cat : book.getCategories()) {
                string lowerCat = cat;
                transform(lowerCat.begin(), lowerCat.end(), lowerCat.begin(), ::tolower);
                if (lowerCat.find(lowerQuery) != string::npos) {
                    found = true;
                    break;
                }
            }
        }
        
        // Search in keywords
        if (!found) {
            for (const string& kw : book.getKeywords()) {
                string lowerKw = kw;
                transform(lowerKw.begin(), lowerKw.end(), lowerKw.begin(), ::tolower);
                if (lowerKw.find(lowerQuery) != string::npos) {
                    found = true;
                    break;
                }
            }
        }
        
        if (found) {
            results.push_back(&book);
        }
    }
    return results;
}

// View and Display
void Library::displayAllBooks() const {
    if (books.empty()) {
        cout << "\nNo books in the library.\n";
        return;
    }
    
    cout << "\n===== All Books in Library =====\n";
    for (const Book& book : books) {
        book.displayBookInfo();
        cout << "----------------------------\n";
    }
}

void Library::displayBooksByCategory() const {
    map<string, vector<const Book*>> categoryMap;
    
    for (const Book& book : books) {
        for (const string& category : book.getCategories()) {
            categoryMap[category].push_back(&book);
        }
    }
    
    cout << "\n===== Books by Category =====\n";
    for (const auto& pair : categoryMap) {
        cout << "\nCategory: " << pair.first << " (" << pair.second.size() << " books)\n";
        for (const Book* book : pair.second) {
            cout << "  - " << book->getTitle() << " by " << book->getAuthor() << "\n";
        }
    }
}

void Library::displayBooksByAuthor() const {
    map<string, vector<const Book*>> authorMap;
    
    for (const Book& book : books) {
        authorMap[book.getAuthor()].push_back(&book);
    }
    
    cout << "\n===== Books by Author =====\n";
    for (const auto& pair : authorMap) {
        cout << "\nAuthor: " << pair.first << " (" << pair.second.size() << " books)\n";
        for (const Book* book : pair.second) {
            cout << "  - " << book->getTitle() << "\n";
        }
    }
}

vector<Book*> Library::sortByPopularity() {
    vector<Book*> sortedBooks;
    for (Book& book : books) {
        sortedBooks.push_back(&book);
    }
    
    sort(sortedBooks.begin(), sortedBooks.end(), 
         [](const Book* a, const Book* b) {
             return a->getViewCount() > b->getViewCount();
         });
    
    return sortedBooks;
}

// Data Management
const vector<Book>& Library::getAllBooks() const {
    return books;
}

int Library::getTotalBooks() const {
    return books.size();
}

int Library::getAvailableBooks() const {
    int count = 0;
    for (const Book& book : books) {
        if (book.isAvailable()) {
            count++;
        }
    }
    return count;
}

// File operations
bool Library::saveToFile(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }
    
    for (const Book& book : books) {
        outFile << book.serialize() << "\n";
    }
    
    outFile.close();
    return true;
}

bool Library::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        return false;
    }
    
    books.clear();
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            Book book = Book::deserialize(line);
            books.push_back(book);
            
            // Update nextBookId để tránh trùng ID
            string bookId = book.getBookId();
            if (bookId.length() > 1 && bookId[0] == 'B') {
                int id = stoi(bookId.substr(1));
                if (id >= nextBookId) {
                    nextBookId = id + 1;
                }
            }
        }
    }
    
    inFile.close();
    return !books.empty();
}

