#ifndef BOOKREQUEST_H
#define BOOKREQUEST_H

#include <string>

using namespace std;

enum class RequestStatus { Pending, Approved, Rejected, Fulfilled, Cancelled };

class BookRequest {
private:
    string requestId;
    string userId;
    string bookTitle;
    string bookAuthor;
    string bookISBN;
    string requestDate;
    RequestStatus status;
    string approvalDate;
    string adminNotes;
    string userReason;

public:
    // Constructors
    BookRequest();
    BookRequest(const string& requestId, const string& userId, const string& bookTitle,
                const string& bookAuthor, const string& bookISBN, const string& requestDate,
                const string& userReason = "");

    // Getters
    string getRequestId() const;
    string getUserId() const;
    string getBookTitle() const;
    string getBookAuthor() const;
    string getBookISBN() const;
    string getRequestDate() const;
    RequestStatus getStatus() const;
    string getApprovalDate() const;
    string getAdminNotes() const;
    string getUserReason() const;

    // Setters
    void setRequestId(const string& id);
    void setUserId(const string& id);
    void setBookTitle(const string& title);
    void setBookAuthor(const string& author);
    void setBookISBN(const string& isbn);
    void setRequestDate(const string& date);
    void setStatus(RequestStatus newStatus);
    void setApprovalDate(const string& date);
    void setAdminNotes(const string& notes);

    // Operations
    void approveRequest(const string& approvalDate, const string& notes = "");
    void rejectRequest(const string& notes);
    void cancelRequest();
    void fulfill();
    void displayRequestInfo() const;

    // Utility functions
    string statusToString() const;
    static RequestStatus stringToStatus(const string& statusStr);

    // Serialization
    string serialize() const;
    static BookRequest deserialize(const string& line);
};

#endif
