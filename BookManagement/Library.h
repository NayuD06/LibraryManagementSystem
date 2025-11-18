#include "Book.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Library {
private:
	vector<Book> books;
public:
	void addBook(const Book& book);

	void updateBook(const string& ISBN, const string& newTitle, const string& newAuthor,
	const string& newPublisher, int newYear, const string& newCategory, const vector<string>& newKeywords, int newQuantity,
	const string& newDescription, double newPrice);

	// Check book status
	void trackBookStatus(const string& ISBN);

	// Remove book
	void removeBook (const string& ISBN);

	// Search book
	void searchBook(const string& keyword);

	void displayAllBooks();
}
