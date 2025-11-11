#include <iostream>
#include "UserManagement/Services/UserService.h"
#include "BookManagement/Services/BookService.h"
#include "BorrowingProcess/Services/BorrowService.h"
#include "AdminAndReporting/Services/AdminService.h"

using namespace std;

int main() {
    cout << "===== Library Management System =====\n";
    cout << "1. User Management\n";
    cout << "2. Book Management\n";
    cout << "3. Borrowing Process\n";
    cout << "4. Admin & Reporting\n";
    cout << "5. Exit\n";
}
