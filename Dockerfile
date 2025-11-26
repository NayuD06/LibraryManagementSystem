# Library Management System - Docker Image
# ⚠️ LƯU Ý: Console app không phù hợp với Docker (input/output issues)
# Khuyến nghị: Build và chạy trực tiếp bằng g++ thay vì Docker

FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy all source files
COPY main.cpp ./
COPY UserManagement/ ./UserManagement/
COPY BookManagement/ ./BookManagement/
COPY BorrowingProcess/ ./BorrowingProcess/
COPY AdminAndReporting/ ./AdminAndReporting/

# Create database directory and copy data files
RUN mkdir -p database
COPY database/users.txt ./database/
COPY database/books.txt ./database/
COPY database/orders.txt ./database/
COPY database/requests.txt ./database/

# Compile the application
RUN g++ -std=c++11 \
    main.cpp \
    UserManagement/User.cpp \
    UserManagement/Services/UserService.cpp \
    BookManagement/Book.cpp \
    BookManagement/Library.cpp \
    BorrowingProcess/Models/Order.cpp \
    BorrowingProcess/Models/Request.cpp \
    AdminAndReporting/Reports/ReportGenerator.cpp \
    -o LibraryManagement

# Run the application
CMD ["./LibraryManagement"]
