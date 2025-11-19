#include "User.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

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

// Get current timestamp
static string getCurrentTimestamp() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

// Constructors
User::User() : id(0), gender(Gender::Other), role(Role::Reader), isActive(true) {
    registrationDate = getCurrentTimestamp();
}

User::User(int id, const string& fullName, const string& dateOfBirth, Gender gender,
           const string& address, const string& phoneNumber, const string& email,
           const string& password, Role role)
    : id(id), fullName(fullName), dateOfBirth(dateOfBirth), gender(gender),
      address(address), phoneNumber(phoneNumber), email(email), role(role), isActive(true) {
    this->password = encryptPassword(password);
    registrationDate = getCurrentTimestamp();
    addActivity("Registration", "User account created");
}

// Getters
int User::getId() const { return id; }
string User::getFullName() const { return fullName; }
string User::getDateOfBirth() const { return dateOfBirth; }
Gender User::getGender() const { return gender; }
string User::getAddress() const { return address; }
string User::getPhoneNumber() const { return phoneNumber; }
string User::getEmail() const { return email; }
Role User::getRole() const { return role; }
const vector<ActivityLog>& User::getActivityHistory() const { return activityHistory; }
string User::getRegistrationDate() const { return registrationDate; }
bool User::getIsActive() const { return isActive; }

// Setters
void User::setFullName(const string& name) { 
    fullName = name; 
    addActivity("Profile Update", "Changed full name");
}

void User::setDateOfBirth(const string& dob) { 
    dateOfBirth = dob; 
    addActivity("Profile Update", "Changed date of birth");
}

void User::setGender(Gender g) { 
    gender = g; 
    addActivity("Profile Update", "Changed gender");
}

void User::setAddress(const string& addr) { 
    address = addr; 
    addActivity("Profile Update", "Changed address");
}

void User::setPhoneNumber(const string& phone) { 
    phoneNumber = phone; 
    addActivity("Profile Update", "Changed phone number");
}

void User::setEmail(const string& mail) { 
    email = mail; 
    addActivity("Profile Update", "Changed email");
}

void User::setPassword(const string& newPassword) {
    password = encryptPassword(newPassword);
    addActivity("Security", "Password changed");
}

void User::setIsActive(bool active) { 
    isActive = active; 
}

// Password management
bool User::checkPassword(const string& plainPassword) const {
    return decryptPassword(password) == plainPassword;
}

string User::getEncryptedPassword() const {
    return password;
}

// Activity tracking
void User::addActivity(const string& action, const string& details) {
    activityHistory.push_back(ActivityLog(action, details, getCurrentTimestamp()));
}

void User::displayActivityHistory() const {
    cout << "\n===== Activity History for " << fullName << " =====\n";
    if (activityHistory.empty()) {
        cout << "No activity recorded.\n";
        return;
    }
    for (const auto& log : activityHistory) {
        cout << "[" << log.timestamp << "] " << log.action << ": " << log.details << "\n";
    }
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

string User::genderToString() const {
    switch (gender) {
        case Gender::Male: return "Male";
        case Gender::Female: return "Female";
        case Gender::Other: return "Other";
        default: return "Unknown";
    }
}

Role User::stringToRole(const string& roleStr) {
    if (roleStr == "Librarian") return Role::Librarian;
    if (roleStr == "Admin") return Role::Admin;
    return Role::Reader;
}

Gender User::stringToGender(const string& genderStr) {
    if (genderStr == "Male") return Gender::Male;
    if (genderStr == "Female") return Gender::Female;
    return Gender::Other;
}

// Display user information
void User::displayUserInfo() const {
    cout << "\n===== User Information =====\n";
    cout << "ID: " << id << "\n";
    cout << "Full Name: " << fullName << "\n";
    cout << "Email: " << email << "\n";
    cout << "Date of Birth: " << dateOfBirth << "\n";
    cout << "Gender: " << genderToString() << "\n";
    cout << "Address: " << address << "\n";
    cout << "Phone: " << phoneNumber << "\n";
    cout << "Role: " << roleToString() << "\n";
    cout << "Registration Date: " << registrationDate << "\n";
    cout << "Account Status: " << (isActive ? "Active" : "Inactive") << "\n";
}

// Serialization for file storage (CSV format)
string User::serialize() const {
    stringstream ss;
    ss << id << "|" << fullName << "|" << dateOfBirth << "|" << genderToString() << "|"
       << address << "|" << phoneNumber << "|" << email << "|" << password << "|"
       << roleToString() << "|" << registrationDate << "|" << (isActive ? "1" : "0");
    return ss.str();
}

User User::deserialize(const string& line) {
    stringstream ss(line);
    string idStr, fullName, dob, genderStr, addr, phone, email, pwd, roleStr, regDate, activeStr;
    
    getline(ss, idStr, '|');
    getline(ss, fullName, '|');
    getline(ss, dob, '|');
    getline(ss, genderStr, '|');
    getline(ss, addr, '|');
    getline(ss, phone, '|');
    getline(ss, email, '|');
    getline(ss, pwd, '|');
    getline(ss, roleStr, '|');
    getline(ss, regDate, '|');
    getline(ss, activeStr, '|');
    
    User user;
    user.id = stoi(idStr);
    user.fullName = fullName;
    user.dateOfBirth = dob;
    user.gender = stringToGender(genderStr);
    user.address = addr;
    user.phoneNumber = phone;
    user.email = email;
    user.password = pwd; // Already encrypted
    user.role = stringToRole(roleStr);
    user.registrationDate = regDate;
    user.isActive = (activeStr == "1");
    
    return user;
}
