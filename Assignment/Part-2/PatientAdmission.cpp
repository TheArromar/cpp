// Role 1: PATIENT ADMISSION CLERK
// Student: MOHAMMED ABDULBAQI MOHAMMED AHMED AQLAN (TP079915)

#include "PatientAdmission.h"
#include <limits>

static PatientQueue patientQueue;

static void seedSamplePatients() {
    Patient p1{1001, "Ali Hassan", "Flu"};
    Patient p2{1002, "John Lim", "Fracture"};
    Patient p3{1003, "Siti Nur", "Chest Pain"};
    patientQueue.enqueue(p1);
    patientQueue.enqueue(p2);
    patientQueue.enqueue(p3);
}

static void admitPatient() {
    Patient p;

    std::cout << "\n--- 1. Admit Patient ---\n";

    std::cout << "Enter Patient ID (integer): ";
    while (!(std::cin >> p.id)) {
        std::cout << "Invalid input. Please enter an integer ID: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Patient Name        : ";
    std::getline(std::cin, p.name);

    std::cout << "Enter Condition Type      : ";
    std::getline(std::cin, p.conditionType);

    if (!patientQueue.enqueue(p)) {
        std::cout << "\n[ALERT] Patient queue is full. Cannot admit more patients.\n";
    } else {
        std::cout << "\n[SUCCESS] Patient admitted into waiting queue.\n";
    }
}

static void dischargePatient() {
    std::cout << "\n--- 2. Discharge Earliest Patient ---\n";
    Patient served;
    if (!patientQueue.dequeue(served)) {
        std::cout << "\n[INFO] No patients to discharge.\n";
        return;
    }

    std::cout << "Discharging patient:\n";
    std::cout << "  ID       : " << served.id << "\n";
    std::cout << "  Name     : " << served.name << "\n";
    std::cout << "  Condition: " << served.conditionType << "\n";
}

static void viewPatientQueueMenu() {
    std::cout << "\n--- 3. View Patient Queue ---\n";
    patientQueue.displayAll();
}

void runPatientAdmissionClerk() {
    int choice;
    static bool seeded = false;

    if (!seeded) {
        seedSamplePatients();
        seeded = true;
    }

    do {
        std::cout << "\n==============================================\n";
        std::cout << "   PATIENT ADMISSION CLERK - Queue Menu\n";
        std::cout << "==============================================\n";
        std::cout << "1. Admit Patient (Enqueue)\n";
        std::cout << "2. Discharge Earliest Patient (Dequeue)\n";
        std::cout << "3. View Patient Queue\n";
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
            admitPatient();
            break;
        case 2:
            dischargePatient();
            break;
        case 3:
            viewPatientQueueMenu();
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
