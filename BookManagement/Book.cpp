#include "Book.h"
#include <iostream>
using namespace std;

// Constructor
Book::Book (string title, string author, int publicationYear, string ISBN, string category, vector<string> keywords, int quantity, string description, double price)
:title(title), author(author), publisher(publisher),
 publicationYear(year), ISBN(isbn), category(category),
 keywords(keywords), quantity(quantity), availableQuantity(quantity),
 status("Available"), description(description), price(price);

 void Book::displaybookInfor(){
	 cout << "Title: " << title << "\n";
	 cout << "Author: " << author << "\n";
	 cout << "Publisher: " << publisher << "\n";
	 cout << "Year: "<< publicationYear <<"\n";
	 cout << "ISBN: "<< ISBN<< "\n";
	 cout << "Available copies: "<< availableQuantity <<"\n";
	 cout << "Status: " << status <<"\n";
	 cout <<"Decription: " << description <<"\n";
	 cout <<"Price: " << price << "\n";

	 // Use vectors to push keywords
	 cout <<"keywords: ";
	 for (const auto& kw: keywords){
		 cout << kw << " ";
	 }
	 void Book::updateStatus(const string& newStatus) {
    status = newStatus;
}

bool Book::isAvailable() const {
    return availableQuantity > 0;
}
// Borrow book
void Book:decreaseAvailableQuantity(){
	if  (availableQuantity > 0){ // Check if at least one copy is available
		availableQuantity--;  // Borrow one copy -> decrease by 1
		if(availableQuantity==0){
			status = "Borrowed"; // Update status if no copy is left
		}
	}
}
// Return book
void Book::increaseAvailableQuantity(){ 
	if(availableQuantity < quantity){ // Check if we can return without exceeding total quantity
		availableQuantity++; // Increase available copies by 1
		status = "Available";  // Update status
	}
}
// Setters
void Book::setTitle(const string& newTitle) { title = newTitle; }
void Book::setAuthor(const string& newAuthor) { author = newAuthor; }
void Book::setPublisher(const string& newPublisher) { publisher = newPublisher; }
void Book::setPublicationYear(int year) { publicationYear = year; }
void Book::setISBN(const string& newISBN) { ISBN = newISBN; }
void Book::setCategory(const string& newCategory) { category = newCategory; }
void Book::setKeywords(const vector<string>& newKeywords) { keywords = newKeywords; }
void Book::setQuantity(int newQuantity) { quantity = newQuantity; }
void Book::setDescription(const string& newDescription) { description = newDescription; }
void Book::setPrice(double newPrice){price = newPrice;}

 }

