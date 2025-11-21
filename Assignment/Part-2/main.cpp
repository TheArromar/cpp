// Group: Hospital Patient Care Management System (Core Data Structures)
// Main Integrator & Team Leader: ARROMAR A/L IRISHIGESAN (TP070288)

#include <iostream>
#include <limits>

// Role 1: Patient Admission Clerk
void runPatientAdmissionClerk();

// Role 2: Medical Supply Manager
void runMedicalSupplyManager();

// Role 3: Emergency Department Officer
void runEmergencyDepartmentOfficer();

// Role 4: Ambulance Dispatcher
void runAmbulanceDispatcher();

int main() {
    int choice;

    do {
        std::cout << "=============================================================\n";
        std::cout << "  Hospital Patient Care Management System (Core Data Structures)\n";
        std::cout << "=============================================================\n";
        std::cout << "1. Patient Admission Clerk\n";
        std::cout << "2. Medical Supply Manager\n";
        std::cout << "3. Emergency Department Officer\n";
        std::cout << "4. Ambulance Dispatcher\n";
        std::cout << "0. Exit System\n";
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            runPatientAdmissionClerk();
            break;
        case 2:
            runMedicalSupplyManager();
            break;
        case 3:
            runEmergencyDepartmentOfficer();
            break;
        case 4:
            runAmbulanceDispatcher();
            break;
        case 0:
            std::cout << "\nExiting system. Thank you.\n";
            break;
        default:
            std::cout << "\n[ERROR] Invalid choice. Please try again.\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
