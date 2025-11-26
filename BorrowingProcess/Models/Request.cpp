#include "Request.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructors
Request::Request() 
    : status(RequestStatus::Pending), queuePosition(0) {}

Request::Request(const string& requestId, const string& userId, 
                         const string& bookId, const string& requestDate, 
                         const string& expiryDate)
    : requestId(requestId), userId(userId), bookId(bookId),
      requestDate(requestDate), expiryDate(expiryDate),
      status(RequestStatus::Pending), queuePosition(0) {}

// Getters
string Request::getRequestId() const { return requestId; }
string Request::getUserId() const { return userId; }
string Request::getBookId() const { return bookId; }
string Request::getRequestDate() const { return requestDate; }
RequestStatus Request::getStatus() const { return status; }
string Request::getExpiryDate() const { return expiryDate; }
string Request::getFulfillmentDate() const { return fulfillmentDate; }
int Request::getQueuePosition() const { return queuePosition; }

// Setters
void Request::setRequestId(const string& id) { requestId = id; }
void Request::setUserId(const string& id) { userId = id; }
void Request::setBookId(const string& id) { bookId = id; }
void Request::setRequestDate(const string& date) { requestDate = date; }
void Request::setStatus(RequestStatus newStatus) { status = newStatus; }
void Request::setExpiryDate(const string& date) { expiryDate = date; }
void Request::setFulfillmentDate(const string& date) { fulfillmentDate = date; }
void Request::setQueuePosition(int position) { queuePosition = position; }

// Operations
void Request::confirmRequest() {
    status = RequestStatus::Confirmed;
    cout << "Request " << requestId << " confirmed.\n";
}

void Request::cancelRequest() {
    status = RequestStatus::Cancelled;
    cout << "Request " << requestId << " cancelled.\n";
}

void Request::markAsExpired() {
    status = RequestStatus::Expired;
    cout << "Request " << requestId << " has expired.\n";
}

void Request::fulfill() {
    status = RequestStatus::Fulfilled;
    cout << "Request " << requestId << " fulfilled.\n";
}

bool Request::isActive() const {
    return status == RequestStatus::Pending || 
           status == RequestStatus::Confirmed;
}

void Request::displayRequestInfo() const {
    cout << "\n===== Request Information =====\n";
    cout << "Request ID: " << requestId << "\n";
    cout << "User ID: " << userId << "\n";
    cout << "Book ID: " << bookId << "\n";
    cout << "Request Date: " << requestDate << "\n";
    cout << "Status: " << statusToString() << "\n";
    cout << "Expiry Date: " << expiryDate << "\n";
    if (queuePosition > 0) {
        cout << "Queue Position: " << queuePosition << "\n";
    }
    if (!fulfillmentDate.empty()) {
        cout << "Fulfilled On: " << fulfillmentDate << "\n";
    }
}

// Utility functions
string Request::statusToString() const {
    switch (status) {
        case RequestStatus::Pending: return "Pending";
        case RequestStatus::Confirmed: return "Confirmed";
        case RequestStatus::Cancelled: return "Cancelled";
        case RequestStatus::Expired: return "Expired";
        case RequestStatus::Fulfilled: return "Fulfilled";
        default: return "Unknown";
    }
}

RequestStatus Request::stringToStatus(const string& statusStr) {
    if (statusStr == "Pending") return RequestStatus::Pending;
    if (statusStr == "Confirmed") return RequestStatus::Confirmed;
    if (statusStr == "Cancelled") return RequestStatus::Cancelled;
    if (statusStr == "Expired") return RequestStatus::Expired;
    if (statusStr == "Fulfilled") return RequestStatus::Fulfilled;
    return RequestStatus::Pending;
}

// Serialization
string Request::serialize() const {
    stringstream ss;
    ss << requestId << "|" << userId << "|" << bookId << "|"
       << requestDate << "|" << statusToString() << "|" << expiryDate << "|"
       << fulfillmentDate << "|" << queuePosition;
    return ss.str();
}

Request Request::deserialize(const string& line) {
    stringstream ss(line);
    string reqId, userId, bookId, reqDate, statusStr, expDate, fulfDate, queueStr;
    
    getline(ss, reqId, '|');
    getline(ss, userId, '|');
    getline(ss, bookId, '|');
    getline(ss, reqDate, '|');
    getline(ss, statusStr, '|');
    getline(ss, expDate, '|');
    getline(ss, fulfDate, '|');
    getline(ss, queueStr, '|');
    
    Request request(reqId, userId, bookId, reqDate, expDate);
    request.status = stringToStatus(statusStr);
    request.fulfillmentDate = fulfDate;
    request.queuePosition = queueStr.empty() ? 0 : stoi(queueStr);
    
    return request;
}
