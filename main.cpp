#include "gym_membership.h"
#include <iostream>
#include <limits>

int main() {
    int choice;

    // Load members from the file when the program starts
    loadMembersFromFile();

    do {
        displayMainMenu();
        std::cout << "Enter your choice: ";

        while (!(std::cin >> choice)) {
            // If the user enters a non-integer, clear the input buffer and display an error message
            std::cout << "Invalid input. Please enter a valid integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                createAdminAccount();
                break;
            case 3:
                std::cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
