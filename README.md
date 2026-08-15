\# Smart Queue Management System



A C++ and MySQL based queue management system designed to manage customer appointments efficiently using priority-based queueing.



\## Problem Statement



Traditional appointment and queue systems can cause unnecessary waiting and inefficient customer handling.



This project provides a simple system where customers can book appointments, urgent customers can receive higher priority, and staff can manage the queue through different appointment states.



\## Features



\- Add new customers

\- Book appointments

\- Generate unique queue tokens

\- Priority-based customer queue

\- Call the next customer

\- Complete appointments

\- Cancel appointments

\- Search customers

\- View customer appointment history

\- Generate daily appointment reports

\- Persistent data storage using MySQL



\## Technologies Used



\- C++

\- MySQL

\- MySQL Connector/C++

\- SQL

\- Object-Oriented Programming concepts

\- STL `priority\_queue`



\## Data Structures



The project uses C++ STL `priority\_queue` to manage the waiting queue.



Customers with urgent priority are served before normal customers.



If two customers have the same priority, the customer with the lower token number is served first.



\### Queue Ordering



```text

URGENT + lower token

&#x20;       ↓

URGENT + higher token

&#x20;       ↓

NORMAL + lower token

&#x20;       ↓

NORMAL + higher token

