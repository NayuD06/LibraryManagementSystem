#include "BookRequest.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructors
BookRequest::BookRequest() : status(RequestStatus::Pending) {}

BookRequest::BookRequest(const string& requestId, const string& userId, 
                         const string& bookTitle, const string& bookAuthor,
                         const string& bookISBN, const string& requestDate,
                         const string& userReason)
    : requestId(requestId), userId(userId), bookTitle(bookTitle),
      bookAuthor(bookAuthor), bookISBN(bookISBN), requestDate(requestDate),
      userReason(userReason), status(RequestStatus::Pending) {}

// Getters
string BookRequest::getRequestId() const { return requestId; }
string BookRequest::getUserId() const { return userId; }
string BookRequest::getBookTitle() const { return bookTitle; }
string BookRequest::getBookAuthor() const { return bookAuthor; }
string BookRequest::getBookISBN() const { return bookISBN; }
string BookRequest::getRequestDate() const { return requestDate; }
RequestStatus BookRequest::getStatus() const { return status; }
string BookRequest::getApprovalDate() const { return approvalDate; }
string BookRequest::getAdminNotes() const { return adminNotes; }
string BookRequest::getUserReason() const { return userReason; }

// Setters
void BookRequest::setRequestId(const string& id) { requestId = id; }
void BookRequest::setUserId(const string& id) { userId = id; }
void BookRequest::setBookTitle(const string& title) { bookTitle = title; }
void BookRequest::setBookAuthor(const string& author) { bookAuthor = author; }
void BookRequest::setBookISBN(const string& isbn) { bookISBN = isbn; }
void BookRequest::setRequestDate(const string& date) { requestDate = date; }
void BookRequest::setStatus(RequestStatus newStatus) { status = newStatus; }
void BookRequest::setApprovalDate(const string& date) { approvalDate = date; }
void BookRequest::setAdminNotes(const string& notes) { adminNotes = notes; }

// Operations
void BookRequest::approveRequest(const string& approvalDate, const string& notes) {
    status = RequestStatus::Approved;
    this->approvalDate = approvalDate;
    this->adminNotes = notes;
    cout << "Book request " << requestId << " approved.\n";
}

void BookRequest::rejectRequest(const string& notes) {
    status = RequestStatus::Rejected;
    this->adminNotes = notes;
    cout << "Book request " << requestId << " rejected.\n";
}

void BookRequest::cancelRequest() {
    status = RequestStatus::Cancelled;
    cout << "Book request " << requestId << " cancelled by user.\n";
}

void BookRequest::fulfill() {
    status = RequestStatus::Fulfilled;
    cout << "Book request " << requestId << " fulfilled.\n";
}

void BookRequest::displayRequestInfo() const {
    cout << "\n===== Book Request Information =====\n";
    cout << "Request ID: " << requestId << "\n";
    cout << "User ID: " << userId << "\n";
    cout << "Book Title: " << bookTitle << "\n";
    cout << "Author: " << bookAuthor << "\n";
    if (!bookISBN.empty()) {
        cout << "ISBN: " << bookISBN << "\n";
    }
    cout << "Request Date: " << requestDate << "\n";
    cout << "Status: " << statusToString() << "\n";
    if (!userReason.empty()) {
        cout << "User Reason: " << userReason << "\n";
    }
    if (!approvalDate.empty()) {
        cout << "Approval Date: " << approvalDate << "\n";
    }
    if (!adminNotes.empty()) {
        cout << "Admin Notes: " << adminNotes << "\n";
    }
}

// Utility functions
string BookRequest::statusToString() const {
    switch (status) {
        case RequestStatus::Pending: return "Pending";
        case RequestStatus::Approved: return "Approved";
        case RequestStatus::Rejected: return "Rejected";
        case RequestStatus::Fulfilled: return "Fulfilled";
        case RequestStatus::Cancelled: return "Cancelled";
        default: return "Unknown";
    }
}

RequestStatus BookRequest::stringToStatus(const string& statusStr) {
    if (statusStr == "Pending") return RequestStatus::Pending;
    if (statusStr == "Approved") return RequestStatus::Approved;
    if (statusStr == "Rejected") return RequestStatus::Rejected;
    if (statusStr == "Fulfilled") return RequestStatus::Fulfilled;
    if (statusStr == "Cancelled") return RequestStatus::Cancelled;
    return RequestStatus::Pending;
}

// Serialization
string BookRequest::serialize() const {
    stringstream ss;
    ss << requestId << "|" << userId << "|" << bookTitle << "|" << bookAuthor << "|"
       << bookISBN << "|" << requestDate << "|" << statusToString() << "|"
       << approvalDate << "|" << adminNotes << "|" << userReason;
    return ss.str();
}

BookRequest BookRequest::deserialize(const string& line) {
    stringstream ss(line);
    string reqId, userId, title, author, isbn, reqDate, statusStr, appDate, notes, reason;
    
    getline(ss, reqId, '|');
    getline(ss, userId, '|');
    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, isbn, '|');
    getline(ss, reqDate, '|');
    getline(ss, statusStr, '|');
    getline(ss, appDate, '|');
    getline(ss, notes, '|');
    getline(ss, reason, '|');
    
    BookRequest request(reqId, userId, title, author, isbn, reqDate, reason);
    request.status = stringToStatus(statusStr);
    request.approvalDate = appDate;
    request.adminNotes = notes;
    
    return request;
}
