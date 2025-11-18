using namespace std;
#include <string>
#include <vector>
#include <iostream>
class Book{
private:
	string title;
	string author;
	string publisher;
	int publicationYear;
	string ISBN; 
	string category;
	vector<string> keywords;
	int quantity;
	int availableQuantity;
	string status;
	string description;
	double price;
	
	
public:
// Constructor
	Book (string title, string author, int publicationYear, string ISBN, string category, vector<string> keywords, int quantity, string description, double price);
	


// Getters
	string getTitle() { return title;}
	string getAuthor(){return author;}
	string getPublisher(){return publisher;}
	int getPublicationYear() {return publicationYear;}
	string getISBN(){return ISBN;}
	string getCategory(){return category;}
	const vector<string>& getKeywords(){return keywords;}
	int getQuantity() {return quantity;}
	int getAvailableQuantity() {return availableQuantity;}
	string getStatus() {return status;}
	string getDescription() {return description;}
	double getPrice(){return price;}
	
// Setters	
	void setTitle(const string& newTitle);
    void setAuthor(const string& newAuthor);
    void setPublisher(const string& newPublisher);
    void setPublicationYear(int year);
    void setISBN(const string& newISBN);
    void setCategory(const string& newCategory);
    void setKeywords(const vector<string>& newKeywords);
    void setQuantity(int newQuantity);
    void setDescription(const string& newDescription);
	void setPrice(double newPrice);
	
	void displayBookInfo() const;
    void updateStatus(const string& newStatus);
    bool isAvailable() const;
    void decreaseAvailableQuantity(); // Decrease quantity of book
    void increaseAvailableQuantity(); // Increase quantity of book