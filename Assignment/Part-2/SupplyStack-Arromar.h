// Role 2: MEDICAL SUPPLY MANAGER - Data Structure
// Student: ARROMAR A/L IRISHIGESAN (TP070288)

#pragma once

#include <iostream>
#include <iomanip>
#include <string>

struct SupplyItem {
    std::string name;        // e.g. "Surgical Mask"
    std::string category;    // e.g. "PPE", "Medicine", "Equipment"
    int quantity;            // e.g. 100
    std::string expiryDate;  // e.g. "2026-05-30"
};

struct SupplyNode {
    SupplyItem data;
    SupplyNode* next;

    SupplyNode(const SupplyItem& item)
        : data(item), next(nullptr) {}
};

// Linked-list based stack to manage medical supply items (LIFO).
class SupplyStack {
private:
    int size;
    SupplyNode* top;

public:
    SupplyStack() : top(nullptr), size(0) {}

    ~SupplyStack() {
        clear();
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }

    void push(const SupplyItem& item) {
        SupplyNode* newNode = new SupplyNode(item);
        newNode->next = top;
        top = newNode;
        ++size;
    }

    bool pop(SupplyItem& outItem) {
        if (isEmpty()) {
            return false;
        }
        SupplyNode* temp = top;
        outItem = top->data;
        top = top->next;
        delete temp;
        --size;
        return true;
    }

    bool peek(SupplyItem& outItem) const {
        if (isEmpty()) {
            return false;
        }
        outItem = top->data;
        return true;
    }

    void clear() {
        while (!isEmpty()) {
            SupplyNode* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }

    // Display from last-added (top) downwards
    void displayAll() const {
        if (isEmpty()) {
            std::cout << "\n[INFO] No medical supplies in stock.\n";
            return;
        }

        std::cout << "\n===============================================================\n";
        std::cout << "   CURRENT MEDICAL SUPPLY INVENTORY (Top = Last Added)\n";
        std::cout << "===============================================================\n";
        std::cout << std::left
                    << std::setw(4)  << "No."
                    << std::setw(22) << "Name"
                    << std::setw(16) << "Category"
                    << std::setw(10) << "Quantity"
                    << std::setw(14) << "Expiry\n";
        std::cout << "---------------------------------------------------------------\n";

        const SupplyNode* curr = top;
        int index = 1;
        while (curr != nullptr) {
            const SupplyItem& item = curr->data;
            std::cout << std::left
                        << std::setw(4)  << index
                        << std::setw(22) << item.name
                        << std::setw(16) << item.category
                        << std::setw(10) << item.quantity
                        << std::setw(14) << item.expiryDate
                        << "\n";
            curr = curr->next;
            ++index;
        }

        std::cout << "===============================================================\n";
        std::cout << "Total items in stack: " << size << "\n";
    }
};
