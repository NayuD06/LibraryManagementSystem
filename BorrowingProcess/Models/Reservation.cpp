#include "Reservation.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructors
Reservation::Reservation() 
    : status(ReservationStatus::Pending), queuePosition(0) {}

Reservation::Reservation(const string& reservationId, const string& userId, 
                         const string& bookId, const string& reservationDate, 
                         const string& expiryDate)
    : reservationId(reservationId), userId(userId), bookId(bookId),
      reservationDate(reservationDate), expiryDate(expiryDate),
      status(ReservationStatus::Pending), queuePosition(0) {}

// Getters
string Reservation::getReservationId() const { return reservationId; }
string Reservation::getUserId() const { return userId; }
string Reservation::getBookId() const { return bookId; }
string Reservation::getReservationDate() const { return reservationDate; }
ReservationStatus Reservation::getStatus() const { return status; }
string Reservation::getExpiryDate() const { return expiryDate; }
string Reservation::getFulfillmentDate() const { return fulfillmentDate; }
int Reservation::getQueuePosition() const { return queuePosition; }

// Setters
void Reservation::setReservationId(const string& id) { reservationId = id; }
void Reservation::setUserId(const string& id) { userId = id; }
void Reservation::setBookId(const string& id) { bookId = id; }
void Reservation::setReservationDate(const string& date) { reservationDate = date; }
void Reservation::setStatus(ReservationStatus newStatus) { status = newStatus; }
void Reservation::setExpiryDate(const string& date) { expiryDate = date; }
void Reservation::setFulfillmentDate(const string& date) { fulfillmentDate = date; }
void Reservation::setQueuePosition(int position) { queuePosition = position; }

// Operations
void Reservation::confirmReservation() {
    status = ReservationStatus::Confirmed;
    cout << "Reservation " << reservationId << " confirmed.\n";
}

void Reservation::cancelReservation() {
    status = ReservationStatus::Cancelled;
    cout << "Reservation " << reservationId << " cancelled.\n";
}

void Reservation::markAsExpired() {
    status = ReservationStatus::Expired;
    cout << "Reservation " << reservationId << " has expired.\n";
}

void Reservation::fulfill() {
    status = ReservationStatus::Fulfilled;
    cout << "Reservation " << reservationId << " fulfilled.\n";
}

bool Reservation::isActive() const {
    return status == ReservationStatus::Pending || 
           status == ReservationStatus::Confirmed;
}

void Reservation::displayReservationInfo() const {
    cout << "\n===== Reservation Information =====\n";
    cout << "Reservation ID: " << reservationId << "\n";
    cout << "User ID: " << userId << "\n";
    cout << "Book ID: " << bookId << "\n";
    cout << "Reservation Date: " << reservationDate << "\n";
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
string Reservation::statusToString() const {
    switch (status) {
        case ReservationStatus::Pending: return "Pending";
        case ReservationStatus::Confirmed: return "Confirmed";
        case ReservationStatus::Cancelled: return "Cancelled";
        case ReservationStatus::Expired: return "Expired";
        case ReservationStatus::Fulfilled: return "Fulfilled";
        default: return "Unknown";
    }
}

ReservationStatus Reservation::stringToStatus(const string& statusStr) {
    if (statusStr == "Pending") return ReservationStatus::Pending;
    if (statusStr == "Confirmed") return ReservationStatus::Confirmed;
    if (statusStr == "Cancelled") return ReservationStatus::Cancelled;
    if (statusStr == "Expired") return ReservationStatus::Expired;
    if (statusStr == "Fulfilled") return ReservationStatus::Fulfilled;
    return ReservationStatus::Pending;
}

// Serialization
string Reservation::serialize() const {
    stringstream ss;
    ss << reservationId << "|" << userId << "|" << bookId << "|"
       << reservationDate << "|" << statusToString() << "|" << expiryDate << "|"
       << fulfillmentDate << "|" << queuePosition;
    return ss.str();
}

Reservation Reservation::deserialize(const string& line) {
    stringstream ss(line);
    string resId, userId, bookId, resDate, statusStr, expDate, fulfDate, queueStr;
    
    getline(ss, resId, '|');
    getline(ss, userId, '|');
    getline(ss, bookId, '|');
    getline(ss, resDate, '|');
    getline(ss, statusStr, '|');
    getline(ss, expDate, '|');
    getline(ss, fulfDate, '|');
    getline(ss, queueStr, '|');
    
    Reservation reservation(resId, userId, bookId, resDate, expDate);
    reservation.status = stringToStatus(statusStr);
    reservation.fulfillmentDate = fulfDate;
    reservation.queuePosition = queueStr.empty() ? 0 : stoi(queueStr);
    
    return reservation;
}
