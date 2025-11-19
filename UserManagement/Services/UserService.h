#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "../User.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class UserService {
private:
    vector<User> users;
    int nextUserId;
    User* currentUser;

    // Helper functions
    string generateUserId();
    bool emailExists(const string& email) const;

public:
    UserService();

    // User Registration and Authentication
    bool registerUser(const string& name, const string& email, const string& password,
                     const string& phoneNumber, Role role = Role::Reader);
    
    User* login(const string& email, const string& password);
    void logout();
    User* getCurrentUser();
    
    // User Management
    User* findUserById(int userId);
    User* findUserByEmail(const string& email);
    bool updateUserProfile(int userId, const string& name, const string& email, const string& phoneNumber);
    bool changePassword(int userId, const string& oldPassword, const string& newPassword);
    bool deactivateUser(int userId);
    bool activateUser(int userId);
    
    // Admin Functions
    vector<User*> getAllUsers();
    vector<User*> getUsersByRole(Role role);
    void displayAllUsers() const;
    
    // File Operations
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
};

#endif
