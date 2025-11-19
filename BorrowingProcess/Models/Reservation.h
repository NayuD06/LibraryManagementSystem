#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

using namespace std;

enum class ReservationStatus { Pending, Confirmed, Cancelled, Expired, Fulfilled };

class Reservation {
private:
    string reservationId;
    string userId;
    string bookId;
    string reservationDate;
    ReservationStatus status;
    string expiryDate;
    string fulfillmentDate;
    int queuePosition;

public:
    // Constructors
    Reservation();
    Reservation(const string& reservationId, const string& userId, const string& bookId,
                const string& reservationDate, const string& expiryDate);

    // Getters
    string getReservationId() const;
    string getUserId() const;
    string getBookId() const;
    string getReservationDate() const;
    ReservationStatus getStatus() const;
    string getExpiryDate() const;
    string getFulfillmentDate() const;
    int getQueuePosition() const;

    // Setters
    void setReservationId(const string& id);
    void setUserId(const string& id);
    void setBookId(const string& id);
    void setReservationDate(const string& date);
    void setStatus(ReservationStatus newStatus);
    void setExpiryDate(const string& date);
    void setFulfillmentDate(const string& date);
    void setQueuePosition(int position);

    // Operations
    void confirmReservation();
    void cancelReservation();
    void markAsExpired();
    void fulfill();
    bool isActive() const;
    void displayReservationInfo() const;

    // Utility functions
    string statusToString() const;
    static ReservationStatus stringToStatus(const string& statusStr);

    // Serialization
    string serialize() const;
    static Reservation deserialize(const string& line);
};

#endif
