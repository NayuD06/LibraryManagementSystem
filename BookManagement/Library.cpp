#include "Library.h"
#include <algorithm>
using namespace std;
 
// Add book
 void Library::addBook(const Book& book){
	 books.push_back(book); // Add the new book to the vector
	 cout << "Book" << book.getTitle() <<"added. \n";
 }
 // Update book
 void Library::updateBook(const string& ISBN, const string& newTitle, const string& newAuthor,
	const string& newPublisher, int newYear, const string& newCategory, const vector<string>& newKeywords, int newQuantity,
	const string& newDescription, double newPrice)
    {
    for (Book& book : books) { // Loop through all books (Range Based method)
    if (book.getISBN() == ISBN) { // Find book by ISBN
        book.setTitle(newTitle);
        book.setAuthor(newAuthor);
        book.setPublisher(newPublisher);
        book.setPublicationYear(newYear);
        book.setCategory(newCategory);
        book.setKeywords(newKeywords);
        book.setQuantity(newQuantity);
        book.setDescription(newDescription);
        cout << "Book with ISBN " << ISBN << " updated.\n";
        return;
    }
}
cout << "Book with ISBN " << ISBN << " not found.\n";}

// Check book status
void Library::trackBookStatus(const string& ISBN){
    for (Book& book : books){
        if (book.getISBN() == ISBN){ // Find book by ISBN
            cout << "Book" << book.getTitle() <<"status: "<< book.getAvailableQuantity() << "available out of "<< book.getQuantity()<<"\n";
            return;
        }
    }
    cout<< "Book with ISBN " << ISBN << " not found \n";
}
// Remove book
void Library::removeBook(const string& ISBN) {
    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].getISBN() == ISBN) {
            books.erase(books.begin() + i);
            cout << "Book with ISBN " << ISBN << " removed.\n";
            return;
        }
    }
    cout << "Book with ISBN " << ISBN << " not found.\n";
}
void Library::searchBook(const string& keyword) {
    bool found = false; // Check if there are any matching books

    // Loop through all books in the library (Range Based method)
    for (const Book& book : books) {
        // Check if keyword exists in the title, author, or category
        if (book.getTitle().find(keyword) != string::npos ||
            book.getAuthor().find(keyword) != string::npos ||
            book.getCategory().find(keyword) != string::npos) {
            
            book.displayBookInfo();  // Display book information
            cout << "----------------------\n";
            found = true;
        } else {
            // Check if keyword exists in the book's keywords
            for (const string& kw : book.getKeywords()) {
                if (kw.find(keyword) != string::npos) {
                    book.displayBookInfo();
                    cout << "----------------------\n";
                    found = true; // Mark as found
                    break; // Stop if found in keyword
                }
            }
        }
    }

    if (!found) {
        cout << "No books found matching '" << keyword << "'.\n";
    }
}
