// Role 3: EMERGENCY DEPARTMENT OFFICER
// Student: ABDULLAH MANAR MAATOQ ABDO (TP073641)

#pragma once

#include <iostream>
#include <iomanip>
#include <string>

struct EmergencyCase {
    std::string patientName;
    std::string emergencyType;
    int priority; // Larger number = more critical
};

// Array-based max-heap priority queue
class EmergencyPriorityQueue {
private:
    static const int MAX_CASES = 100;
    EmergencyCase heap[MAX_CASES];
    int size;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int index) {
        while (index != 0 && heap[parent(index)].priority < heap[index].priority) {
            std::swap(heap[parent(index)], heap[index]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = leftChild(index);
            int right = rightChild(index);
            int largest = index;

            if (left < size && heap[left].priority > heap[largest].priority)
                largest = left;
            if (right < size && heap[right].priority > heap[largest].priority)
                largest = right;

            if (largest == index) break;

            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    EmergencyPriorityQueue() : size(0) {}

    bool isEmpty() const { return size == 0; }
    bool isFull() const { return size == MAX_CASES; }

    bool insertCase(const EmergencyCase& c) {
        if (isFull()) return false;
        heap[size] = c;
        heapifyUp(size);
        ++size;
        return true;
    }

    bool removeMostCritical(EmergencyCase& out) {
        if (isEmpty()) return false;
        out = heap[0];
        heap[0] = heap[size - 1];
        --size;
        heapifyDown(0);
        return true;
    }

    bool peekMostCritical(EmergencyCase& out) const {
        if (isEmpty()) return false;
        out = heap[0];
        return true;
    }

    void displayAll() const {
        if (isEmpty()) {
            std::cout << "\n[INFO] No emergency cases in the queue.\n";
            return;
        }

        std::cout << "\n========== PENDING EMERGENCY CASES (by Priority) ==========\n";
        std::cout << std::left
                  << std::setw(4)  << "No."
                  << std::setw(20) << "Patient"
                  << std::setw(20) << "Emergency Type"
                  << std::setw(10) << "Priority\n";
        std::cout << "------------------------------------------------------------\n";

        // Display in heap order (near-priority order)
        for (int i = 0; i < size; ++i) {
            const EmergencyCase& c = heap[i];
            std::cout << std::left
                      << std::setw(4)  << (i + 1)
                      << std::setw(20) << c.patientName
                      << std::setw(20) << c.emergencyType
                      << std::setw(10) << c.priority
                      << "\n";
        }

        std::cout << "------------------------------------------------------------\n";
        std::cout << "Total pending cases: " << size << "\n";
    }
};
