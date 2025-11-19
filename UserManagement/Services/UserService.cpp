#include "UserService.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

UserService::UserService() : nextUserId(1), currentUser(nullptr) {}

string UserService::generateUserId() {
    stringstream ss;
    ss << setfill('0') << setw(5) << nextUserId++;
    return ss.str();
}

bool UserService::emailExists(const string& email) const {
    for (const User& user : users) {
        if (user.getEmail() == email) {
            return true;
        }
    }
    return false;
}

// User Registration and Authentication
bool UserService::registerUser(const string& fullName, const string& dateOfBirth, 
                               Gender gender, const string& address, const string& phoneNumber,
                               const string& email, const string& password, Role role) {
    if (emailExists(email)) {
        cout << "Error: Email already exists!\n";
        return false;
    }
    
    User newUser(nextUserId++, fullName, dateOfBirth, gender, address, phoneNumber, 
                 email, password, role);
    users.push_back(newUser);
    
    cout << "User registered successfully!\n";
    cout << "User ID: " << newUser.getId() << "\n";
    cout << "Email: " << email << "\n";
    return true;
}

User* UserService::login(const string& email, const string& password) {
    for (User& user : users) {
        if (user.getEmail() == email) {
            if (!user.getIsActive()) {
                cout << "Error: Account is deactivated!\n";
                return nullptr;
            }
            if (user.checkPassword(password)) {
                currentUser = &user;
                user.addActivity("Login", "User logged in");
                cout << "Login successful! Welcome, " << user.getFullName() << "!\n";
                return &user;
            } else {
                cout << "Error: Incorrect password!\n";
                return nullptr;
            }
        }
    }
    cout << "Error: User not found!\n";
    return nullptr;
}

void UserService::logout() {
    if (currentUser) {
        currentUser->addActivity("Logout", "User logged out");
        cout << "Logged out successfully!\n";
        currentUser = nullptr;
    }
}

User* UserService::getCurrentUser() {
    return currentUser;
}

// User Management
User* UserService::findUserById(int userId) {
    for (User& user : users) {
        if (user.getId() == userId) {
            return &user;
        }
    }
    return nullptr;
}

User* UserService::findUserByEmail(const string& email) {
    for (User& user : users) {
        if (user.getEmail() == email) {
            return &user;
        }
    }
    return nullptr;
}

bool UserService::updateUserProfile(int userId, const string& fullName, 
                                   const string& dateOfBirth, Gender gender,
                                   const string& address, const string& phoneNumber) {
    User* user = findUserById(userId);
    if (user) {
        user->setFullName(fullName);
        user->setDateOfBirth(dateOfBirth);
        user->setGender(gender);
        user->setAddress(address);
        user->setPhoneNumber(phoneNumber);
        cout << "Profile updated successfully!\n";
        return true;
    }
    cout << "Error: User not found!\n";
    return false;
}

bool UserService::changePassword(int userId, const string& oldPassword, 
                                const string& newPassword) {
    User* user = findUserById(userId);
    if (user) {
        if (user->checkPassword(oldPassword)) {
            user->setPassword(newPassword);
            cout << "Password changed successfully!\n";
            return true;
        } else {
            cout << "Error: Incorrect old password!\n";
            return false;
        }
    }
    cout << "Error: User not found!\n";
    return false;
}

bool UserService::deactivateUser(int userId) {
    User* user = findUserById(userId);
    if (user) {
        user->setIsActive(false);
        cout << "User account deactivated.\n";
        return true;
    }
    return false;
}

bool UserService::activateUser(int userId) {
    User* user = findUserById(userId);
    if (user) {
        user->setIsActive(true);
        cout << "User account activated.\n";
        return true;
    }
    return false;
}

// Activity Tracking
void UserService::logUserActivity(int userId, const string& action, const string& details) {
    User* user = findUserById(userId);
    if (user) {
        user->addActivity(action, details);
    }
}

void UserService::displayUserActivity(int userId) const {
    for (const User& user : users) {
        if (user.getId() == userId) {
            user.displayActivityHistory();
            return;
        }
    }
    cout << "User not found!\n";
}

// Admin Functions
vector<User*> UserService::getAllUsers() {
    vector<User*> userList;
    for (User& user : users) {
        userList.push_back(&user);
    }
    return userList;
}

vector<User*> UserService::getUsersByRole(Role role) {
    vector<User*> userList;
    for (User& user : users) {
        if (user.getRole() == role) {
            userList.push_back(&user);
        }
    }
    return userList;
}

void UserService::displayAllUsers() const {
    if (users.empty()) {
        cout << "\nNo users in the system.\n";
        return;
    }
    
    cout << "\n===== All Users =====\n";
    for (const User& user : users) {
        user.displayUserInfo();
        cout << "----------------------------\n";
    }
}

// File Operations
bool UserService::saveToFile(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }
    
    for (const User& user : users) {
        outFile << user.serialize() << "\n";
    }
    
    outFile.close();
    cout << "User data saved to " << filename << "\n";
    return true;
}

bool UserService::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Warning: Could not open file " << filename << " for reading.\n";
        return false;
    }
    
    users.clear();
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            users.push_back(User::deserialize(line));
            if (users.back().getId() >= nextUserId) {
                nextUserId = users.back().getId() + 1;
            }
        }
    }
    
    inFile.close();
    cout << "Loaded " << users.size() << " users from " << filename << "\n";
    return true;
}
