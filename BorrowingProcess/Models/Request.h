#ifndef REQUEST_H
#define REQUEST_H

#include <string>

using namespace std;

enum class RequestStatus { Pending, Confirmed, Cancelled, Expired, Fulfilled };

class Request {
private:
    string requestId;
    string userId;
    string bookId;
    string requestDate;
    RequestStatus status;
    string expiryDate;
    string fulfillmentDate;
    int queuePosition;

public:
    // Constructors
    Request();
    Request(const string& requestId, const string& userId, const string& bookId,
                const string& requestDate, const string& expiryDate);

    // Getters
    string getRequestId() const;
    string getUserId() const;
    string getBookId() const;
    string getRequestDate() const;
    RequestStatus getStatus() const;
    string getExpiryDate() const;
    string getFulfillmentDate() const;
    int getQueuePosition() const;

    // Setters
    void setRequestId(const string& id);
    void setUserId(const string& id);
    void setBookId(const string& id);
    void setRequestDate(const string& date);
    void setStatus(RequestStatus newStatus);
    void setExpiryDate(const string& date);
    void setFulfillmentDate(const string& date);
    void setQueuePosition(int position);

    // Operations
    void confirmRequest();
    void cancelRequest();
    void markAsExpired();
    void fulfill();
    bool isActive() const;
    void displayRequestInfo() const;

    // Utility functions
    string statusToString() const;
    static RequestStatus stringToStatus(const string& statusStr);

    // Serialization
    string serialize() const;
    static Request deserialize(const string& line);
};

#endif
