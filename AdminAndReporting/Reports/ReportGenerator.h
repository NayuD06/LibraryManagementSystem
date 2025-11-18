#pragma once
#include <vector>
#include "../Services/AdminService.h"

class ReportGenerator {
public:
    void generalStats(const std::vector<User>& users, const std::vector<Book>& books);
    void inventory(const std::vector<Book>& books);
    void userList(const std::vector<User>& users);
};
