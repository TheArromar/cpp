// Role 4: AMBULANCE DISPATCHER
// Student: DHIREN REEF SEBASTIAN (TP079271)

#pragma once

#include <iostream>
#include <string>

struct Ambulance {
    std::string id;
};

// Circular queue for ambulance rotation
class AmbulanceCircularQueue {
private:
    static const int MAX_AMBULANCES = 50;
    Ambulance data[MAX_AMBULANCES];
    int front;
    int rear;
    int count;

public:
    AmbulanceCircularQueue() : front(0), rear(-1), count(0) {}

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == MAX_AMBULANCES; }

    bool enqueue(const Ambulance& a) {
        if (isFull()) return false;
        rear = (rear + 1) % MAX_AMBULANCES;
        data[rear] = a;
        ++count;
        return true;
    }

    // Rotate: move front ambulance to the back
    bool rotate() {
        if (count <= 1) return false;
        Ambulance first = data[front];
        front = (front + 1) % MAX_AMBULANCES;
        rear = (rear + 1) % MAX_AMBULANCES;
        data[rear] = first;
        return true;
    }

    void displaySchedule() const {
        if (isEmpty()) {
            std::cout << "\n[INFO] No ambulances registered.\n";
            return;
        }

        std::cout << "\n========== CURRENT AMBULANCE SCHEDULE ==========\n";
        int index = front;
        for (int i = 0; i < count; ++i) {
            std::cout << "Slot " << (i + 1)
                      << " -> Ambulance ID: " << data[index].id << "\n";
            index = (index + 1) % MAX_AMBULANCES;
        }
        std::cout << "================================================\n";
    }
};
