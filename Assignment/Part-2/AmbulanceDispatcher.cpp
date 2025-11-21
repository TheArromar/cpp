// Role 4: AMBULANCE DISPATCHER
// Student: DHIREN REEF SEBASTIAN (TP079271)

#include "AmbulanceDispatcher.h"
#include <limits>

static AmbulanceCircularQueue ambulanceQueue;

static void seedSampleAmbulances() {
    Ambulance a1{ "A1" };
    Ambulance a2{ "B1" };
    Ambulance a3{ "C1" };
    ambulanceQueue.enqueue(a1);
    ambulanceQueue.enqueue(a2);
    ambulanceQueue.enqueue(a3);
}

static void registerAmbulance() {
    Ambulance a;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n--- 1. Register Ambulance ---\n";
    std::cout << "Enter Ambulance ID: ";
    std::getline(std::cin, a.id);

    if (!ambulanceQueue.enqueue(a)) {
        std::cout << "\n[ALERT] Cannot register more ambulances. Schedule is full.\n";
    } else {
        std::cout << "\n[SUCCESS] Ambulance " << a.id << " registered.\n";
    }
}

static void rotateAmbulanceShiftMenu() {
    std::cout << "\n--- 2. Rotate Ambulance Shift ---\n";
    if (!ambulanceQueue.rotate()) {
        std::cout << "[INFO] Not enough ambulances to rotate.\n";
    } else {
        std::cout << "[SUCCESS] Ambulance shift rotated.\n";
    }
}

static void displayAmbulanceScheduleMenu() {
    std::cout << "\n--- 3. Display Ambulance Schedule ---\n";
    ambulanceQueue.displaySchedule();
}

void runAmbulanceDispatcher() {
    int choice;
    static bool seeded = false;

    if (!seeded) {
        seedSampleAmbulances();
        seeded = true;
    }

    do {
        std::cout << "\n==============================================\n";
        std::cout << "   AMBULANCE DISPATCHER - Rotation Menu\n";
        std::cout << "==============================================\n";
        std::cout << "1. Register Ambulance (Enqueue)\n";
        std::cout << "2. Rotate Ambulance Shift (Circular Queue Rotate)\n";
        std::cout << "3. Display Ambulance Schedule\n";
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
            registerAmbulance();
            break;
        case 2:
            rotateAmbulanceShiftMenu();
            break;
        case 3:
            displayAmbulanceScheduleMenu();
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
