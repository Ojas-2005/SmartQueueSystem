CREATE DATABASE IF NOT EXISTS smart_queue;

USE smart_queue;

-- ==========================================
-- CUSTOMERS TABLE
-- ==========================================

CREATE TABLE IF NOT EXISTS customers
(
    customer_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(15) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL
);

-- ==========================================
-- APPOINTMENTS TABLE
-- ==========================================

CREATE TABLE IF NOT EXISTS appointments
(
    appointment_id INT AUTO_INCREMENT PRIMARY KEY,

    customer_id INT NOT NULL,

    appointment_date DATE NOT NULL,

    appointment_time TIME NOT NULL,

    token INT NOT NULL UNIQUE,

    priority TINYINT NOT NULL DEFAULT 0,

    status ENUM(
        'WAITING',
        'IN_PROGRESS',
        'COMPLETED',
        'CANCELLED'
    ) NOT NULL DEFAULT 'WAITING',

    CONSTRAINT fk_customer
        FOREIGN KEY (customer_id)
        REFERENCES customers(customer_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

-- ==========================================
-- SAMPLE CUSTOMERS
-- ==========================================

INSERT INTO customers (name, phone, email)
VALUES
('Arjun Mehta', '9876543201', 'arjun.mehta@gmail.com'),
('Neha Sharma', '9876543202', 'neha.sharma@gmail.com'),
('Rohan Gupta', '9876543203', 'rohan.gupta@gmail.com'),
('Priya Singh', '9876543204', 'priya.singh@gmail.com'),
('Aditya Verma', '9876543205', 'aditya.verma@gmail.com'),
('Simran Kaur', '9876543206', 'simran.kaur@gmail.com'),
('Karan Malhotra', '9876543207', 'karan.malhotra@gmail.com'),
('Ananya Kapoor', '9876543208', 'ananya.kapoor@gmail.com'),
('Rahul Joshi', '9876543209', 'rahul.joshi@gmail.com'),
('Ishita Rao', '9876543210', 'ishita.rao@gmail.com');


-- ==========================================
-- SAMPLE APPOINTMENTS
-- ==========================================

INSERT INTO appointments
(
    customer_id,
    appointment_date,
    appointment_time,
    token,
    priority,
    status
)
VALUES
(1, '2026-08-15', '10:00:00', 1, 0, 'WAITING'),
(2, '2026-08-15', '10:15:00', 2, 1, 'WAITING'),
(3, '2026-08-15', '10:30:00', 3, 0, 'WAITING'),
(4, '2026-08-15', '10:45:00', 4, 1, 'WAITING'),
(5, '2026-08-15', '11:00:00', 5, 0, 'WAITING'),
(6, '2026-08-15', '11:15:00', 6, 0, 'WAITING'),
(7, '2026-08-15', '11:30:00', 7, 1, 'WAITING'),
(8, '2026-08-15', '11:45:00', 8, 0, 'IN_PROGRESS'),
(9, '2026-08-15', '09:00:00', 9, 0, 'COMPLETED'),
(10, '2026-08-15', '09:15:00', 10, 1, 'COMPLETED'),
(1, '2026-08-15', '09:30:00', 11, 0, 'COMPLETED'),
(2, '2026-08-15', '09:45:00', 12, 0, 'CANCELLED'),
(3, '2026-08-15', '12:00:00', 13, 1, 'CANCELLED'),
(4, '2026-08-15', '09:50:00', 14, 0, 'COMPLETED'),
(5, '2026-08-15', '12:15:00', 15, 0, 'WAITING');


-- ==========================================
-- VERIFY DATA
-- ==========================================

SELECT * FROM customers;

SELECT * FROM appointments;