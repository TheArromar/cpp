// Role 2: MEDICAL SUPPLY MANAGER - Inventory Logic
// Student: ARROMAR A/L IRISHIGESAN (TP070288)

#include "SupplyStack-Arromar.h"

#include <iostream>
#include <limits>

// Global stack instance for this role
static SupplyStack supplyStack;

// 1. Add Supply Stock
static void addSupplyStock() {
    SupplyItem item;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n--- 1. Add Supply Stock ---\n";
    std::cout << "Enter supply name             : ";
    std::getline(std::cin, item.name);

    std::cout << "Enter category (PPE/Med/Equip): ";
    std::getline(std::cin, item.category);

    std::cout << "Enter quantity (integer)      : ";
    while (!(std::cin >> item.quantity) || item.quantity <= 0) {
        std::cout << "Invalid quantity. Please enter a positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Enter expiry date (e.g. 2026-05-30): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, item.expiryDate);

    supplyStack.push(item);

    std::cout << "\n[SUCCESS] Supply added to inventory (LAST ADDED item).\n";
    std::cout << "Stack size is now: " << supplyStack.getSize() << "\n";
}

// 2. Use 'Last Added' Supply
static void useLastAddedSupply() {
    std::cout << "\n--- 2. Use 'Last Added' Supply ---\n";

    SupplyItem used;
    if (!supplyStack.pop(used)) {
        std::cout << "\n[ALERT] No supplies available to use.\n";
        return;
    }

    std::cout << "The following supply will be used (removed from stock):\n";
    std::cout << "  - Name    : " << used.name << "\n";
    std::cout << "  - Category: " << used.category << "\n";
    std::cout << "  - Quantity: " << used.quantity << "\n";
    std::cout << "  - Expiry  : " << used.expiryDate << "\n";

    std::cout << "\n[INFO] Supply consumed. Remaining items in stack: "
                << supplyStack.getSize() << "\n";
}

// 3. View Current Supplies
static void viewCurrentSupplies() {
    std::cout << "\n--- 3. View Current Supplies ---\n";
    supplyStack.displayAll();
}

// Seed some dummy data for demo/testing
static void seedSampleSupplies() {
    SupplyItem a{ "Surgical Mask",    "PPE",       200, "2026-01-01" };
    SupplyItem b{ "IV Drip Set",      "Equipment",  40, "2027-03-15" };
    SupplyItem c{ "Paracetamol 500mg","Medicine", 120, "2025-12-31" };
    SupplyItem d{ "N95 Respirator",   "PPE",        80, "2026-06-30" };

    supplyStack.push(a);
    supplyStack.push(b);
    supplyStack.push(c);
    supplyStack.push(d);
}

// Role runner 
void runMedicalSupplyManager() {
    int choice;
    static bool seeded = false;

    if (!seeded) {
        seedSampleSupplies();
        seeded = true;
    }

    do {
        std::cout << "\n==============================================\n";
        std::cout << "   MEDICAL SUPPLY MANAGER - Inventory Menu\n";
        std::cout << "==============================================\n";
        std::cout << "1. Add Supply Stock (Push onto Stack)\n";
        std::cout << "2. Use 'Last Added' Supply (Pop from Stack)\n";
        std::cout << "3. View Current Supplies (Display Stack)\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "----------------------------------------------\n";
        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            addSupplyStock();
            break;
        case 2:
            useLastAddedSupply();
            break;
        case 3:
            viewCurrentSupplies();
            break;
        case 0:
            std::cout << "\nReturning to Main Menu...\n";
            break;
        default:
            std::cout << "\n[ERROR] Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 0);
}
