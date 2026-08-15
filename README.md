Smart Queue Management System

A C++ and MySQL based queue management system designed to manage customer appointments efficiently using priority-based queueing.

Problem Statement

Traditional appointment and queue systems can cause unnecessary waiting and inefficient customer handling.

This project provides a simple system where customers can book appointments, urgent customers receive higher priority, and staff can manage appointments through different stages.

Features
Add new customers
Book appointments
Generate unique queue tokens
Priority-based customer queue
Call the next customer
Complete appointments
Cancel appointments
Search customers by ID or phone
View customer appointment history
Generate daily appointment reports
Persistent data storage using MySQL
Technologies Used
C++
MySQL
MySQL Connector/C++
SQL
C++ STL
priority_queue
Custom comparator
Data Structures and Algorithms

The project uses C++ STL priority_queue to manage the waiting queue.

A custom comparator determines which customer should be served first.

Queue Priority Rules
Urgent customers are served before normal customers.
If two customers have the same priority, the customer with the lower token number is served first.

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

Example

Token 2 -> URGENT
Token 4 -> URGENT
Token 1 -> NORMAL
Token 3 -> NORMAL

The queue will serve:

Token 2
Token 4
Token 1
Token 3

Database Design

The system contains two main tables:

Customers

Stores customer information.
| Column      | Description           |
| ----------- | --------------------- |
| customer_id | Unique customer ID    |
| name        | Customer name         |
| phone       | Customer phone number |
| email       | Customer email        |


Appointments

Stores appointment and queue information.
| Column           | Description            |
| ---------------- | ---------------------- |
| appointment_id   | Unique appointment ID  |
| customer_id      | Reference to customer  |
| appointment_date | Appointment date       |
| appointment_time | Appointment time       |
| token            | Queue token            |
| priority         | 0 = Normal, 1 = Urgent |
| status           | Appointment status     |

Database Relationship

The database uses a one-to-many relationship.
CUSTOMERS
    |
    | 1
    |
    | many
    v
APPOINTMENTS

One customer can have multiple appointments.

The customer_id column in the appointments table is a foreign key referencing the customers table.

Appointment Status Flow

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

SQL Concepts Used

The project demonstrates several SQL concepts:

SELECT
INSERT
UPDATE
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

Example SQL JOIN

Customer appointment history is retrieved using a JOIN:
SELECT
    a.appointment_id,
    c.name,
    a.token,
    a.status
FROM appointments a
JOIN customers c
ON a.customer_id = c.customer_id;

Daily Report

The system generates statistics such as:

Total appointments
Waiting appointments
In-progress appointments
Completed appointments
Cancelled appointments
Urgent appointments
Normal appointments

Example:
Total Appointments : 15
Waiting            : 8
In Progress        : 1
Completed          : 4
Cancelled          : 2
Urgent Appointments: 4
Normal Appointments: 11

Project Structure

SmartQueueSystem/
|
|-- main.cpp
|-- database.sql
|-- README.md
`-- .gitignore

Requirements
Windows
Visual Studio Build Tools
C++ compiler
MySQL Server
MySQL Connector/C++
Database Setup
Install MySQL Server.
Install MySQL Connector/C++.
Open MySQL Workbench.
Run database.sql.
The script creates the smart_queue database and required tables.
Configuration

The MySQL password is not stored directly in the source code.

The application reads the password from the Windows environment variable:

SMART_QUEUE_DB_PASSWORD

Set it before running the application:
setx SMART_QUEUE_DB_PASSWORD "MY_PASSWORD"

Open a new terminal after setting the variable.

Compilation

Using the Visual Studio x64 Native Tools Command Prompt:

cl /EHsc /MD /I"C:\Program Files\MySQL\MySQL Connector C++ 26.7\include" main.cpp /link /LIBPATH:"C:\Program Files\MySQL\MySQL Connector C++ 26.7\lib64\vs14" mysqlcppconnx.lib

Run the application:
main.exe

Main Menu
1. Add Customer
2. Book Appointment
3. View Current Queue
4. Call Next Customer
5. Complete Appointment
6. Cancel Appointment
7. Search Customer
8. Daily Report
9. Exit

Future Improvements

Possible future improvements include:

GUI interface
Login and role-based access
Multiple service counters
Real-time queue display
Appointment reminders
Web-based interface
REST API
Analytics dashboard
Author

Ojas Lamba

BCA Student