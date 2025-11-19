#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

enum class Role { Reader, Librarian, Admin };
enum class Gender { Male, Female, Other };

// Activity log entry for tracking user actions
struct ActivityLog {
    string action;      // e.g., "Login", "Borrow", "Return", "Reserve", "Search"
    string details;     // Additional information
    string timestamp;   // Date and time of activity
    
    ActivityLog(const string& act, const string& det, const string& ts)
        : action(act), details(det), timestamp(ts) {}
};

class User {
private:
    int id;
    string fullName;
    string dateOfBirth;
    Gender gender;
    string address;
    string phoneNumber;
    string email;           // Used as login name
    string password;        // Encrypted password
    Role role;
    vector<ActivityLog> activityHistory;
    string registrationDate;
    bool isActive;

    // Helper function for password encryption (simple XOR for demo)
    static string encryptPassword(const string& plain);
    static string decryptPassword(const string& encrypted);

public:
    // Constructors
    User();
    User(int id, const string& fullName, const string& dateOfBirth, Gender gender,
         const string& address, const string& phoneNumber, const string& email,
         const string& password, Role role = Role::Reader);

    // Getters
    int getId() const;
    string getFullName() const;
    string getDateOfBirth() const;
    Gender getGender() const;
    string getAddress() const;
    string getPhoneNumber() const;
    string getEmail() const;
    Role getRole() const;
    const vector<ActivityLog>& getActivityHistory() const;
    string getRegistrationDate() const;
    bool getIsActive() const;

    // Setters for updating personal information
    void setFullName(const string& name);
    void setDateOfBirth(const string& dob);
    void setGender(Gender g);
    void setAddress(const string& addr);
    void setPhoneNumber(const string& phone);
    void setEmail(const string& mail);
    void setPassword(const string& newPassword);
    void setIsActive(bool active);

    // Password management
    bool checkPassword(const string& plainPassword) const;
    string getEncryptedPassword() const;

    // Activity tracking
    void addActivity(const string& action, const string& details);
    void displayActivityHistory() const;

    // Utility functions
    string roleToString() const;
    string genderToString() const;
    static Role stringToRole(const string& roleStr);
    static Gender stringToGender(const string& genderStr);

    // Display user information
    void displayUserInfo() const;

    // Serialization for file storage
    string serialize() const;
    static User deserialize(const string& line);
};

#endif
