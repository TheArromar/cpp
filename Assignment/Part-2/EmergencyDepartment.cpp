// Role 3: EMERGENCY DEPARTMENT OFFICER
// Student: ABDULLAH MANAR MAATOQ ABDO (TP073641)

#include "EmergencyDepartment.h"
#include <limits>

static EmergencyPriorityQueue emergencyQueue;

static void seedSampleEmergencies() {
    EmergencyCase c1{ "Kumar", "Heart Attack", 5 };
    EmergencyCase c2{ "Aisha", "Fracture", 2 };
    EmergencyCase c3{ "Lim",   "Stroke", 4 };
    EmergencyCase c4{ "Sara",  "Breathing Difficulty", 3 };

    emergencyQueue.insertCase(c1);
    emergencyQueue.insertCase(c2);
    emergencyQueue.insertCase(c3);
    emergencyQueue.insertCase(c4);
}

static void logEmergencyCase() {
    EmergencyCase c;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n--- 1. Log Emergency Case ---\n";
    std::cout << "Enter patient name          : ";
    std::getline(std::cin, c.patientName);

    std::cout << "Enter type of emergency     : ";
    std::getline(std::cin, c.emergencyType);

    std::cout << "Enter priority (1-5, 5 = most critical): ";
    while (!(std::cin >> c.priority) || c.priority < 1 || c.priority > 5) {
        std::cout << "Invalid priority. Please enter 1-5: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (!emergencyQueue.insertCase(c)) {
        std::cout << "\n[ALERT] Cannot log new case. Priority queue is full.\n";
    } else {
        std::cout << "\n[SUCCESS] Emergency case logged.\n";
    }
}

static void processMostCriticalCase() {
    std::cout << "\n--- 2. Process Most Critical Case ---\n";
    EmergencyCase c;
    if (!emergencyQueue.removeMostCritical(c)) {
        std::cout << "\n[INFO] No emergency cases to process.\n";
        return;
    }

    std::cout << "Attending to emergency case:\n";
    std::cout << "  Patient   : " << c.patientName << "\n";
    std::cout << "  Emergency : " << c.emergencyType << "\n";
    std::cout << "  Priority  : " << c.priority << "\n";
}

static void viewPendingEmergencyCases() {
    std::cout << "\n--- 3. View Pending Emergency Cases ---\n";
    emergencyQueue.displayAll();
}

void runEmergencyDepartmentOfficer() {
    int choice;
    static bool seeded = false;

    if (!seeded) {
        seedSampleEmergencies();
        seeded = true;
    }

    do {
        std::cout << "\n==============================================\n";
        std::cout << "   EMERGENCY DEPARTMENT OFFICER - Priority Menu\n";
        std::cout << "==============================================\n";
        std::cout << "1. Log Emergency Case (Insert into Priority Queue)\n";
        std::cout << "2. Process Most Critical Case (Remove Max Priority)\n";
        std::cout << "3. View Pending Emergency Cases\n";
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
            logEmergencyCase();
            break;
        case 2:
            processMostCriticalCase();
            break;
        case 3:
            viewPendingEmergencyCases();
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
