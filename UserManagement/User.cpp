#include "User.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// Simple XOR encryption for demonstration (use bcrypt/argon2 in production)
string User::encryptPassword(const string& plain) {
    string encrypted = plain;
    for (char& c : encrypted) {
        c = c ^ 0x55; // XOR with key
    }
    return encrypted;
}

string User::decryptPassword(const string& encrypted) {
    return encryptPassword(encrypted); // XOR is reversible
}

// Constructors
User::User() : id(0), role(Role::Reader) {}

User::User(int id, const string& name, const string& email, const string& password,
           const string& phoneNumber, Role role)
    : id(id), name(name), email(email), phoneNumber(phoneNumber), role(role) {
    this->password = encryptPassword(password);
}

// Getters
int User::getId() const { return id; }
string User::getName() const { return name; }
string User::getEmail() const { return email; }
string User::getPhoneNumber() const { return phoneNumber; }
Role User::getRole() const { return role; }

// Setters
void User::setName(const string& n) { 
    name = n;
}

void User::setPhoneNumber(const string& phone) { 
    phoneNumber = phone;
}

void User::setEmail(const string& mail) { 
    email = mail;
}

void User::setPassword(const string& newPassword) {
    password = encryptPassword(newPassword);
}

// Password management
bool User::checkPassword(const string& plainPassword) const {
    return decryptPassword(password) == plainPassword;
}

string User::getEncryptedPassword() const {
    return password;
}

// Utility functions
string User::roleToString() const {
    switch (role) {
        case Role::Reader: return "Reader";
        case Role::Librarian: return "Librarian";
        case Role::Admin: return "Admin";
        default: return "Unknown";
    }
}

Role User::stringToRole(const string& roleStr) {
    if (roleStr == "Librarian") return Role::Librarian;
    if (roleStr == "Admin") return Role::Admin;
    return Role::Reader;
}

// Display user information
void User::displayUserInfo() const {
    cout << "\n===== User Information =====\n";
    cout << "ID: " << id << "\n";
    cout << "Name: " << name << "\n";
    cout << "Email: " << email << "\n";
    cout << "Phone: " << phoneNumber << "\n";
    cout << "Role: " << roleToString() << "\n";
}

// Serialization for file storage (CSV format)
string User::serialize() const {
    stringstream ss;
    ss << id << "|" << name << "|" << email << "|" << password << "|"
       << phoneNumber << "|" << roleToString();
    return ss.str();
}

User User::deserialize(const string& line) {
    stringstream ss(line);
    string idStr, name, email, pwd, phoneNumber, roleStr;
    
    getline(ss, idStr, '|');
    getline(ss, name, '|');
    getline(ss, email, '|');
    getline(ss, pwd, '|');
    getline(ss, phoneNumber, '|');
    getline(ss, roleStr, '|');
    
    User user;
    user.id = stoi(idStr);
    user.name = name;
    user.email = email;
    user.password = pwd; // Already encrypted
    user.phoneNumber = phoneNumber;
    user.role = stringToRole(roleStr);
    
    return user;
}
