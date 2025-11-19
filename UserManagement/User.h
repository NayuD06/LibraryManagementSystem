#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

enum class Role { User, Librarian, Admin };

class User {
private:
    int id;
    string name;
    string email;
    string password;        // Encrypted password
    string phoneNumber;
    Role role;

    // Helper function for password encryption (simple XOR for demo)
    static string encryptPassword(const string& plain);
    static string decryptPassword(const string& encrypted);

public:
    // Constructors
    User();
    User(int id, const string& name, const string& email, const string& password, 
         const string& phoneNumber, Role role = Role::User);

    // Getters
    int getId() const;
    string getName() const;
    string getEmail() const;
    string getPhoneNumber() const;
    Role getRole() const;

    // Setters for updating personal information
    void setName(const string& name);
    void setEmail(const string& mail);
    void setPassword(const string& newPassword);
    void setPhoneNumber(const string& phone);

    // Password management
    bool checkPassword(const string& plainPassword) const;
    string getEncryptedPassword() const;

    // Utility functions
    string roleToString() const;
    static Role stringToRole(const string& roleStr);

    // Display user information
    void displayUserInfo() const;

    // Serialization for file storage
    string serialize() const;
    static User deserialize(const string& line);
};

#endif
