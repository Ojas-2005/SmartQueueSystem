# 🚀 Smart Queue Management System

A C++ and MySQL based queue management system designed to manage customer appointments efficiently using priority-based queueing.

## 📌 Problem Statement

Traditional appointment and queue systems can cause unnecessary waiting and inefficient customer handling.

This project provides a simple system where customers can book appointments, urgent customers receive higher priority, and staff can manage appointments through different stages.

## ✨ Features

- Add new customers
- Book appointments
- Generate unique queue tokens
- Priority-based customer queue
- Call the next customer
- Complete appointments
- Cancel appointments
- Search customers by ID or phone
- View customer appointment history
- Generate daily appointment reports
- Persistent data storage using MySQL

## 🛠️ Technologies Used

- C++
- MySQL
- MySQL Connector/C++
- SQL
- C++ STL
- priority_queue
- Custom comparator

## 🧠 Data Structures and Algorithms

The project uses C++ STL priority_queue to manage the waiting queue.

A custom comparator determines which customer should be served first.

### Queue Priority Rules

1. Urgent customers are served before normal customers.
2. If two customers have the same priority, the customer with the lower token number is served first.

### Queue Ordering

```text
URGENT + lower token
        |
        v
URGENT + higher token
        |
        v
NORMAL + lower token
        |
        v
NORMAL + higher token
```
### Example

- Token 2 -> URGENT
- Token 4 -> URGENT
- Token 1 -> NORMAL
- Token 3 -> NORMAL

The queue will serve:

1. Token 2
2. Token 4
3. Token 1
4. Token 3

## 🗄️ Database Design

The system contains two main tables.

### 👤 Customers Table

Stores customer information.

- `customer_id` - INT - Unique customer ID
- `name` - VARCHAR(100) - Customer name
- `phone` - VARCHAR(15) - Customer phone number
- `email` - VARCHAR(100) - Customer email

### 📅 Appointments Table

Stores appointment and queue information.

- `appointment_id` - INT - Unique appointment ID
- `customer_id` - INT - Reference to customer
- `appointment_date` - DATE - Appointment date
- `appointment_time` - TIME - Appointment time
- `token` - INT - Queue token
- `priority` - TINYINT - 0 = Normal, 1 = Urgent
- `status` - ENUM - Appointment status

### 🔑 Primary Keys

- `customers.customer_id`
- `appointments.appointment_id`

Each appointment also has a unique token.

### 🔗 Foreign Key

The `customer_id` column in the `appointments` table is a foreign key referencing `customers.customer_id`.

## 🔗 Database Relationship

The database uses a one-to-many relationship.

```text
CUSTOMERS
    |
    | 1
    |
    | many
    v
APPOINTMENTS
```

🔄 Appointment Status Flow

A normal appointment follows this flow:

WAITING
   |
   v
IN_PROGRESS
   |
   v
COMPLETED

A waiting appointment can also be cancelled:

WAITING
   |
   v
CANCELLED
Available Statuses
WAITING
IN_PROGRESS
COMPLETED
CANCELLED
🚦 Priority System

The system supports two priority levels.

Priority	Value	Meaning
Normal	0	Regular appointment
Urgent	1	High-priority appointment

Urgent appointments are served before normal appointments.

For appointments with the same priority, the lower token number is served first.

💾 SQL Concepts Used

The project demonstrates several SQL concepts.

CREATE DATABASE
CREATE TABLE
INSERT
SELECT
UPDATE
DELETE
WHERE
JOIN
ORDER BY
COUNT
SUM
CASE
Aggregate functions
Primary keys
Foreign keys
Constraints
AUTO_INCREMENT
ENUM
🔎 Example SQL JOIN

Customer appointment history is retrieved using a JOIN.

SELECT
    a.appointment_id,
    c.name,
    a.token,
    a.status
FROM appointments a
JOIN customers c
ON a.customer_id = c.customer_id;
📊 Daily Report

The system generates statistics such as:

Total appointments
Waiting appointments
In-progress appointments
Completed appointments
Cancelled appointments
Urgent appointments
Normal appointments
Example
Total Appointments : 15
Waiting            : 8
In Progress        : 1
Completed          : 4
Cancelled          : 2
Urgent Appointments: 4
Normal Appointments: 11

The report uses SQL aggregate functions such as:

COUNT()
SUM()
CASE
🔍 Customer Search

Customers can be searched using:

Customer ID
Phone number

The system also displays the customer's complete appointment history, including:

Token
Appointment date
Appointment time
Priority
Status
📋 Queue Management

The queue displays only appointments with the status WAITING.

The C++ priority_queue determines the service order.

Example:

Token    Customer          Priority
------------------------------------
2        Neha Sharma       URGENT
4        Priya Singh       URGENT
7        Karan Malhotra    URGENT
1        Arjun Mehta       NORMAL
3        Rohan Gupta       NORMAL
🔨 Appointment Operations
Add Customer

Stores a new customer in the customers table.

Book Appointment

Creates a new appointment and generates a queue token.

Call Next Customer

Selects the highest-priority waiting appointment and changes its status to IN_PROGRESS.

Complete Appointment

Changes an in-progress appointment to COMPLETED.

Cancel Appointment

Changes a waiting appointment to CANCELLED.

Search Customer

Retrieves customer information and appointment history.

Daily Report

Calculates appointment statistics from the database.

🧩 Project Architecture
                +----------------------+
                |      C++ Program     |
                |                      |
                |  Menu & Application  |
                +----------+-----------+
                           |
                           | MySQL
                           | Connector/C++
                           v
                +----------------------+
                |    MySQL Database    |
                |                      |
                |    smart_queue       |
                +----------+-----------+
                           |
                 +---------+---------+
                 |                   |
                 v                   v
          +-------------+     +--------------+
          |  customers  |     | appointments |
          +-------------+     +--------------+
📁 Project Structure
SmartQueueSystem/
|
|-- main.cpp
|-- database.sql
|-- README.md
`-- .gitignore
⚙️ Requirements
Windows
Visual Studio Build Tools
C++ compiler
MySQL Server
MySQL Connector/C++
MySQL Workbench
🗃️ Database Setup
Install MySQL Server.
Install MySQL Connector/C++.
Open MySQL Workbench.
Open database.sql.
Execute the SQL script.
The script creates the smart_queue database.
The script creates the customers and appointments tables.
Sample customer and appointment data is inserted.
🔐 Configuration

The MySQL password is not stored directly in the source code.

The application reads the password from the Windows environment variable:

SMART_QUEUE_DB_PASSWORD

Set it before running the application:

setx SMART_QUEUE_DB_PASSWORD "YOUR_PASSWORD"

Open a new terminal after setting the variable.

🔨 Compilation

Using the Visual Studio x64 Native Tools Command Prompt:

cl /EHsc /MD /I"C:\Program Files\MySQL\MySQL Connector C++ 26.7\include" main.cpp /link /LIBPATH:"C:\Program Files\MySQL\MySQL Connector C++ 26.7\lib64\vs14" mysqlcppconnx.lib

Run the application:

main.exe
🖥️ Main Menu
1. Add Customer
2. Book Appointment
3. View Current Queue
4. Call Next Customer
5. Complete Appointment
6. Cancel Appointment
7. Search Customer
8. Daily Report
9. Exit
🧪 Sample Data

The project includes sample data containing:

10 customers
15 appointments
Normal appointments
Urgent appointments
Waiting appointments
In-progress appointments
Completed appointments
Cancelled appointments

This sample data allows all major features of the system to be demonstrated.

🔮 Future Improvements

Possible future improvements include:

GUI interface
Login and role-based access
Multiple service counters
Real-time queue display
Appointment reminders
Web-based interface
REST API
Analytics dashboard
Online appointment booking
Admin dashboard
🎯 Learning Outcomes

Through this project, the following concepts were implemented:

C++ programming
STL data structures
Priority queues
Custom comparators
SQL queries
Database design
Primary and foreign keys
One-to-many relationships
CRUD operations
SQL JOINs
Aggregate functions
C++ and MySQL integration
Environment variable based configuration
👨‍💻 Author

Ojas Lamba

BCA Student