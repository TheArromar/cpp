// Role 1: PATIENT ADMISSION CLERK
// Student: MOHAMMED ABDULBAQI MOHAMMED AHMED AQLAN (TP079915)

#pragma once

#include <iostream>
#include <iomanip>
#include <string>

struct Patient {
    int id;
    std::string name;
    std::string conditionType;
};

// Simple array-based FIFO queue for patients
class PatientQueue {
private:
    static const int MAX_PATIENTS = 100;
    Patient data[MAX_PATIENTS];
    int front;
    int rear;
    int count;

public:
    PatientQueue() : front(0), rear(-1), count(0) {}

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == MAX_PATIENTS;
    }

    bool enqueue(const Patient& p) {
        if (isFull()) return false;
        rear = (rear + 1) % MAX_PATIENTS;
        data[rear] = p;
        ++count;
        return true;
    }

    bool dequeue(Patient& out) {
        if (isEmpty()) return false;
        out = data[front];
        front = (front + 1) % MAX_PATIENTS;
        --count;
        return true;
    }

    void displayAll() const {
        if (isEmpty()) {
            std::cout << "\n[INFO] No patients in the queue.\n";
            return;
        }

        std::cout << "\n================ PATIENT QUEUE ================\n";
        std::cout << std::left
                  << std::setw(8)  << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(20) << "Condition\n";
        std::cout << "-----------------------------------------------\n";

        int index = front;
        for (int i = 0; i < count; ++i) {
            const Patient& p = data[index];
            std::cout << std::left
                      << std::setw(8)  << p.id
                      << std::setw(20) << p.name
                      << std::setw(20) << p.conditionType
                      << "\n";

            index = (index + 1) % MAX_PATIENTS;
        }

        std::cout << "-----------------------------------------------\n";
        std::cout << "Total patients waiting: " << count << "\n";
    }
};
