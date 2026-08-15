#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cstdlib>
#include <mysqlx/xdevapi.h>

struct Appointment
{
    int appointmentId;
    int customerId;
    int token;
    int priority;
    std::string name;
};

struct CompareAppointment
{
    bool operator()(const Appointment& a,
                    const Appointment& b)
    {
        if (a.priority != b.priority)
        {
            return a.priority < b.priority;
        }

        return a.token > b.token;
    }
};

void addCustomer(mysqlx::Session& session)
{
    std::string name;
    std::string phone;
    std::string email;

    std::cin.ignore();

    std::cout << "\n===== Add Customer =====\n";

    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    std::cout << "Enter phone: ";
    std::getline(std::cin, phone);

    std::cout << "Enter email: ";
    std::getline(std::cin, email);

    mysqlx::Schema db =
        session.getSchema("smart_queue");

    mysqlx::Table customers =
        db.getTable("customers");

    customers
        .insert("name", "phone", "email")
        .values(name, phone, email)
        .execute();

    std::cout << "\nCustomer added successfully!\n";
}

void bookAppointment(mysqlx::Session& session)
{
    int customerId;
    std::string date;
    std::string time;
    int priority;

    std::cout << "\n===== Book Appointment =====\n";

    std::cout << "Enter Customer ID: ";
    std::cin >> customerId;

    mysqlx::SqlResult customerResult = session.sql(
        "SELECT customer_id "
        "FROM smart_queue.customers "
        "WHERE customer_id = " +
        std::to_string(customerId)
    ).execute();

    bool customerExists = false;

    for (mysqlx::Row row : customerResult)
    {
        customerExists = true;
        break;
    }

    if (!customerExists)
    {
        std::cout << "\nCustomer ID not found.\n";
        std::cout << "Returning to main menu...\n";
        return;
    }

    std::cin.ignore();

    std::cout << "Enter appointment date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    std::cout << "Enter appointment time (HH:MM:SS): ";
    std::getline(std::cin, time);

    std::cout << "Priority appointment? (1 = Yes, 0 = No): ";
    std::cin >> priority;

    if (priority != 0 && priority != 1)
    {
        std::cout << "\nInvalid priority.\n";
        std::cout << "Use 1 for urgent or 0 for normal.\n";
        return;
    }

    mysqlx::Schema db =
        session.getSchema("smart_queue");

    mysqlx::Table appointments =
        db.getTable("appointments");

    mysqlx::RowResult result =
        appointments
            .select("token")
            .orderBy("token DESC")
            .limit(1)
            .execute();

    int token = 1;

    for (mysqlx::Row row : result)
    {
        token = row[0].get<int>() + 1;
    }

    appointments
        .insert(
            "customer_id",
            "appointment_date",
            "appointment_time",
            "token",
            "priority",
            "status"
        )
        .values(
            customerId,
            date,
            time,
            token,
            priority,
            "WAITING"
        )
        .execute();

    std::cout << "\nAppointment booked successfully!\n";
    std::cout << "Token: " << token << '\n';

    if (priority == 1)
    {
        std::cout << "Priority: URGENT\n";
    }
    else
    {
        std::cout << "Priority: NORMAL\n";
    }

    std::cout << "Status: WAITING\n";
}

void viewQueue(mysqlx::Session& session)
{
    mysqlx::SqlResult result = session.sql(
        "SELECT "
        "a.appointment_id, "
        "a.customer_id, "
        "c.name, "
        "a.token, "
        "a.priority "
        "FROM smart_queue.appointments a "
        "JOIN smart_queue.customers c "
        "ON a.customer_id = c.customer_id "
        "WHERE a.status = 'WAITING'"
    ).execute();

    std::priority_queue<
        Appointment,
        std::vector<Appointment>,
        CompareAppointment
    > appointmentQueue;

    for (mysqlx::Row row : result)
    {
        Appointment appointment;

        appointment.appointmentId =
            row[0].get<int>();

        appointment.customerId =
            row[1].get<int>();

        appointment.name =
            row[2].get<std::string>();

        appointment.token =
            row[3].get<int>();

        appointment.priority =
            row[4].get<int>();

        appointmentQueue.push(appointment);
    }

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "           CURRENT QUEUE\n";
    std::cout << "========================================\n\n";

    if (appointmentQueue.empty())
    {
        std::cout << "No waiting appointments.\n";
        return;
    }

    std::cout << "Token    Customer              Priority\n";
    std::cout << "----------------------------------------\n";

    while (!appointmentQueue.empty())
    {
        Appointment current =
            appointmentQueue.top();

        appointmentQueue.pop();

        std::cout << current.token;

        if (current.token < 10)
            std::cout << "        ";
        else
            std::cout << "       ";

        std::cout << current.name;

        if (current.name.length() < 20)
        {
            for (size_t i = current.name.length();
                 i < 20;
                 i++)
            {
                std::cout << " ";
            }
        }

        if (current.priority == 1)
        {
            std::cout << "URGENT\n";
        }
        else
        {
            std::cout << "NORMAL\n";
        }
    }
}

void callNextCustomer(mysqlx::Session& session)
{
    mysqlx::SqlResult result = session.sql(
        "SELECT "
        "a.appointment_id, "
        "a.customer_id, "
        "c.name, "
        "a.token, "
        "a.priority "
        "FROM smart_queue.appointments a "
        "JOIN smart_queue.customers c "
        "ON a.customer_id = c.customer_id "
        "WHERE a.status = 'WAITING'"
    ).execute();

    std::priority_queue<
        Appointment,
        std::vector<Appointment>,
        CompareAppointment
    > appointmentQueue;

    for (mysqlx::Row row : result)
    {
        Appointment appointment;

        appointment.appointmentId =
            row[0].get<int>();

        appointment.customerId =
            row[1].get<int>();

        appointment.name =
            row[2].get<std::string>();

        appointment.token =
            row[3].get<int>();

        appointment.priority =
            row[4].get<int>();

        appointmentQueue.push(appointment);
    }

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "          CALL NEXT CUSTOMER\n";
    std::cout << "========================================\n\n";

    if (appointmentQueue.empty())
    {
        std::cout << "No waiting customers.\n";
        return;
    }

    Appointment nextCustomer =
        appointmentQueue.top();

    std::cout << "Appointment ID: "
              << nextCustomer.appointmentId
              << '\n';

    std::cout << "Token: "
              << nextCustomer.token
              << '\n';

    std::cout << "Customer: "
              << nextCustomer.name
              << '\n';

    if (nextCustomer.priority == 1)
    {
        std::cout << "Priority: URGENT\n";
    }
    else
    {
        std::cout << "Priority: NORMAL\n";
    }

    std::string updateQuery =
        "UPDATE smart_queue.appointments "
        "SET status = 'IN_PROGRESS' "
        "WHERE appointment_id = " +
        std::to_string(nextCustomer.appointmentId);

    session.sql(updateQuery).execute();

    std::cout << "\nCustomer called successfully.\n";
    std::cout << "Status: IN_PROGRESS\n";
}

void completeAppointment(mysqlx::Session& session)
{
    int appointmentId;

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "        COMPLETE APPOINTMENT\n";
    std::cout << "========================================\n\n";

    std::cout << "Enter Appointment ID: ";
    std::cin >> appointmentId;

    mysqlx::SqlResult result = session.sql(
        "SELECT status "
        "FROM smart_queue.appointments "
        "WHERE appointment_id = " +
        std::to_string(appointmentId)
    ).execute();

    bool found = false;
    std::string status;

    for (mysqlx::Row row : result)
    {
        found = true;
        status = row[0].get<std::string>();
    }

    if (!found)
    {
        std::cout << "\nAppointment not found.\n";
        return;
    }

    if (status != "IN_PROGRESS")
    {
        std::cout << "\nAppointment cannot be completed.\n";
        std::cout << "Current status: "
                  << status << '\n';
        return;
    }

    session.sql(
        "UPDATE smart_queue.appointments "
        "SET status = 'COMPLETED' "
        "WHERE appointment_id = " +
        std::to_string(appointmentId)
    ).execute();

    std::cout << "\nAppointment completed successfully!\n";
    std::cout << "Status: COMPLETED\n";
}

void cancelAppointment(mysqlx::Session& session)
{
    int appointmentId;

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "          CANCEL APPOINTMENT\n";
    std::cout << "========================================\n\n";

    std::cout << "Enter Appointment ID: ";
    std::cin >> appointmentId;

    mysqlx::SqlResult result = session.sql(
        "SELECT status "
        "FROM smart_queue.appointments "
        "WHERE appointment_id = " +
        std::to_string(appointmentId)
    ).execute();

    bool found = false;
    std::string status;

    for (mysqlx::Row row : result)
    {
        found = true;
        status = row[0].get<std::string>();
    }

    if (!found)
    {
        std::cout << "\nAppointment not found.\n";
        return;
    }

    if (status != "WAITING")
    {
        std::cout << "\nAppointment cannot be cancelled.\n";
        std::cout << "Current status: "
                  << status << '\n';
        return;
    }

    session.sql(
        "UPDATE smart_queue.appointments "
        "SET status = 'CANCELLED' "
        "WHERE appointment_id = " +
        std::to_string(appointmentId)
    ).execute();

    std::cout << "\nAppointment cancelled successfully!\n";
    std::cout << "Status: CANCELLED\n";
}

void searchCustomer(mysqlx::Session& session)
{
    int choice;

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "            SEARCH CUSTOMER\n";
    std::cout << "========================================\n\n";

    std::cout << "1. Search by Customer ID\n";
    std::cout << "2. Search by Phone\n";
    std::cout << "3. Back\n";

    std::cout << "\nEnter choice: ";
    std::cin >> choice;

    if (choice == 3)
    {
        return;
    }

    if (choice != 1 && choice != 2)
    {
        std::cout << "\nInvalid choice.\n";
        return;
    }

    std::string searchValue;

    if (choice == 1)
    {
        int customerId;

        std::cout << "\nEnter Customer ID: ";
        std::cin >> customerId;

        searchValue = std::to_string(customerId);
    }
    else
    {
        std::cout << "\nEnter Phone: ";
        std::cin >> searchValue;
    }

    std::string query;

    if (choice == 1)
    {
        query =
            "SELECT customer_id, name, phone, email "
            "FROM smart_queue.customers "
            "WHERE customer_id = " +
            searchValue;
    }
    else
    {
        query =
            "SELECT customer_id, name, phone, email "
            "FROM smart_queue.customers "
            "WHERE phone = '" +
            searchValue +
            "'";
    }

    mysqlx::SqlResult customerResult =
        session.sql(query).execute();

    bool found = false;
    int customerId = 0;
    std::string name;
    std::string phone;
    std::string email;

    for (mysqlx::Row row : customerResult)
    {
        found = true;

        customerId =
            row[0].get<int>();

        name =
            row[1].get<std::string>();

        phone =
            row[2].get<std::string>();

        email =
            row[3].get<std::string>();

        break;
    }

    if (!found)
    {
        std::cout << "\nCustomer not found.\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "          CUSTOMER DETAILS\n";
    std::cout << "========================================\n\n";

    std::cout << "Customer ID: " << customerId << '\n';
    std::cout << "Name: " << name << '\n';
    std::cout << "Phone: " << phone << '\n';
    std::cout << "Email: " << email << '\n';

    std::cout << "\nAppointments:\n";
    std::cout << "----------------------------------------\n";

    mysqlx::SqlResult appointmentResult =
        session.sql(
            "SELECT "
            "token, "
            "CAST(appointment_date AS CHAR), "
            "CAST(appointment_time AS CHAR), "
            "priority, "
            "status "
            "FROM smart_queue.appointments "
            "WHERE customer_id = " +
            std::to_string(customerId) +
            " ORDER BY appointment_date DESC, "
            "appointment_time DESC"
        ).execute();

    bool hasAppointments = false;

    for (mysqlx::Row row : appointmentResult)
    {
        hasAppointments = true;

        int token =
            row[0].get<int>();

        std::string date =
            row[1].get<std::string>();

        std::string time =
            row[2].get<std::string>();

        int priority =
            row[3].get<int>();

        std::string status =
            row[4].get<std::string>();

        std::cout << "Token: "
                  << token
                  << " | Date: "
                  << date
                  << " | Time: "
                  << time
                  << " | ";

        if (priority == 1)
        {
            std::cout << "URGENT";
        }
        else
        {
            std::cout << "NORMAL";
        }

        std::cout << " | Status: "
                  << status
                  << '\n';
    }

    if (!hasAppointments)
    {
        std::cout << "No appointments found.\n";
    }
}

void dailyReport(mysqlx::Session& session)
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "             DAILY REPORT\n";
    std::cout << "========================================\n\n";

    mysqlx::SqlResult result = session.sql(
        "SELECT "
        "CAST(COUNT(*) AS SIGNED), "
        "CAST(SUM(CASE WHEN status = 'WAITING' THEN 1 ELSE 0 END) AS SIGNED), "
        "CAST(SUM(CASE WHEN status = 'IN_PROGRESS' THEN 1 ELSE 0 END) AS SIGNED), "
        "CAST(SUM(CASE WHEN status = 'COMPLETED' THEN 1 ELSE 0 END) AS SIGNED), "
        "CAST(SUM(CASE WHEN status = 'CANCELLED' THEN 1 ELSE 0 END) AS SIGNED), "
        "CAST(SUM(CASE WHEN priority = 1 THEN 1 ELSE 0 END) AS SIGNED), "
        "CAST(SUM(CASE WHEN priority = 0 THEN 1 ELSE 0 END) AS SIGNED) "
        "FROM smart_queue.appointments"
    ).execute();

    for (mysqlx::Row row : result)
    {
        int total =
            row[0].get<int>();

        int waiting =
            row[1].get<int>();

        int inProgress =
            row[2].get<int>();

        int completed =
            row[3].get<int>();

        int cancelled =
            row[4].get<int>();

        int urgent =
            row[5].get<int>();

        int normal =
            row[6].get<int>();

        std::cout << "Total Appointments : "
                  << total << '\n';

        std::cout << "Waiting            : "
                  << waiting << '\n';

        std::cout << "In Progress        : "
                  << inProgress << '\n';

        std::cout << "Completed          : "
                  << completed << '\n';

        std::cout << "Cancelled          : "
                  << cancelled << '\n';

        std::cout << "Urgent Appointments: "
                  << urgent << '\n';

        std::cout << "Normal Appointments: "
                  << normal << '\n';
    }

    std::cout << "\n========================================\n";
}

int main()
{
    try
    {
        // Get MySQL password from Windows environment variable
        const char* password =
            std::getenv("SMART_QUEUE_DB_PASSWORD");

        if (password == nullptr)
        {
            std::cout
                << "Database password environment variable not found.\n\n";

            std::cout
                << "Please set SMART_QUEUE_DB_PASSWORD first.\n";

            return 1;
        }

        // Connect to MySQL
        mysqlx::Session session(
            "localhost",
            33060,
            "root",
            password
        );

        std::cout
            << "Connected to MySQL successfully!\n";

        int choice;

        do
        {
            std::cout << "\n";
            std::cout << "========================================\n";
            std::cout << "       SMART QUEUE MANAGEMENT SYSTEM\n";
            std::cout << "========================================\n\n";

            std::cout << "1. Add Customer\n";
            std::cout << "2. Book Appointment\n";
            std::cout << "3. View Current Queue\n";
            std::cout << "4. Call Next Customer\n";
            std::cout << "5. Complete Appointment\n";
            std::cout << "6. Cancel Appointment\n";
            std::cout << "7. Search Customer\n";
            std::cout << "8. Daily Report\n";
            std::cout << "9. Exit\n";

            std::cout << "\nEnter choice: ";
            std::cin >> choice;

            switch (choice)
            {
                case 1:
                    addCustomer(session);
                    break;

                case 2:
                    bookAppointment(session);
                    break;

                case 3:
                    viewQueue(session);
                    break;

                case 4:
                    callNextCustomer(session);
                    break;

                case 5:
                    completeAppointment(session);
                    break;

                case 6:
                    cancelAppointment(session);
                    break;

                case 7:
                    searchCustomer(session);
                    break;

                case 8:
                    dailyReport(session);
                    break;

                case 9:
                    std::cout
                        << "\nExiting program...\n";
                    break;

                default:
                    std::cout
                        << "\nInvalid choice. Try again.\n";
            }

        } while (choice != 9);

        session.close();
    }
    catch (const mysqlx::Error& err)
    {
        std::cout
            << "\nMySQL Error: "
            << err.what()
            << '\n';
    }
    catch (const std::exception& err)
    {
        std::cout
            << "\nError: "
            << err.what()
            << '\n';
    }

    return 0;
}