#include "gym_membership.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <limits>

using namespace std;

// Define global variables
MemberNode* memberList = nullptr;
unordered_map<int, Member> memberMap;

void displayMembersInfo() {
    std::cout << "\n+" << std::string(50, '-') << "+";
    std::cout << "|\n+" << std::string(15, '-') << " MEMBERS INFORMATION " << std::string(14, '-') << "|\n";
    std::cout << "+" << std::string(50, '-') << "+\n";

    MemberNode* currentMember = memberList;
    while (currentMember) {
        std::cout << "\nMember ID: " << currentMember->memberID << "\n";
        std::cout << "Member Name: " << currentMember->memberName << "\n";
        std::cout << "Membership Type: ";

        // Convert membership type character to full name
        switch (currentMember->membershipType) {
            case 'P':
                std::cout << "Premium";
                break;
            case 'N':
                std::cout << "Normal";
                break;
            case 'S':
                std::cout << "Student";
                break;
            default:
                std::cout << "Unknown";
        }

        std::cout << "\n";
        std::cout << "Member Status: " << (currentMember->isMemberActive ? "Active" : "Inactive") << "\n";
        std::cout << "\n===============================\n";
        currentMember = currentMember->next;
    }
}

int generateUniqueMemberID() {
    // Implement your logic to generate a unique member ID
    // For simplicity, you can use a counter, or a more complex logic involving timestamps, etc.
    static int counter = 1;
    return counter++;
}

void displayMainMenu() {
    cout << "\n+" << string(15, '-') << " GYM MEMBERSHIP MANAGEMENT SYSTEM " << string(16, '-') << "+\n";
    cout << "| 1. Admin Login                                                  |\n";
    cout << "+" << string(65, '-') << "+\n";
    cout << "| 2. Create Admin Account                                         |\n";
    cout << "+" << string(65, '-') << "+\n";
    cout <<  "| 3. Exit                                                         |\n";
    cout << "+" << string(65, '-') << "+\n";
}

void loadMembersFromFile() {
    // Clear the existing memberList
    while (memberList) {
        MemberNode* temp = memberList;
        memberList = memberList->next;
        delete temp;
    }

    std::ifstream memberFile("member_credentials.txt");  // Use std::ifstream

    if (memberFile.is_open()) {
        int storedMemberID;
        std::string storedMemberName;
        bool storedIsMemberActive;

        while (memberFile >> storedMemberID >> storedMemberName >> storedIsMemberActive) {
            // Create a new member node
            MemberNode* newMember = new MemberNode;
            newMember->memberID = storedMemberID;
            newMember->memberName = storedMemberName;
            newMember->isMemberActive = storedIsMemberActive;
            newMember->next = nullptr;

            // Add the new member to the linked list
            if (!memberList) {
                memberList = newMember;
            } else {
                MemberNode* currentMember = memberList;
                while (currentMember->next) {
                    currentMember = currentMember->next;
                }
                currentMember->next = newMember;
            }

            // Add the new member to the unordered_map
            memberMap[storedMemberID] = {storedMemberName, storedIsMemberActive};
        }

        memberFile.close();
    } else {
        std::cout << "Error opening file for reading.\n";
    }
}

void adminLogin() {
    string username, password;

    cout << "\n\n+" << string(44, '-') << "+";
    cout << "\n|" << string(15, '-') << " ADMIN LOGIN " << string(16, '-') << "|\n";
    cout << "+" << string(44, '-') << "+\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (isValidAdmin(username, password)) {
        cout << "Admin login successful!\n";
        adminActionsMenu(); // Call admin actions menu
    } else {
        cout << "Invalid username or password. ";
        cout << "Please try again with valid credentials.\n";
    }
}

void adminActionsMenu() {
    int adminChoice;

    do {
        std::cout << "\n\n+" << std::string(42, '-') << "+";
        std::cout << "\n|" << std::string(15, '-') << " ADMIN MENU " << std::string(15, '-') << "|\n";
        std::cout << "+" << std::string(42, '-') << "+\n\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 1. Create Member Account       |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 2. Display Members Information | \n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 3. Update Member Information   |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 4. Delete Member               |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 5. Search Member               |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 6. Sort Members                |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "| 7. Logout                      |\n";
        std::cout << "+" << std::string(33, '-') << "+\n";
        std::cout << "\n Enter your choice: ";

        while (!(std::cin >> adminChoice)) {
            // If the user enters a non-integer, clear the input buffer and display an error message
            std::cout << "Invalid input. Please enter a valid integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (adminChoice) {
            case 1:
                createMemberAccount();
                break;
            case 2:
                displayMembersInfo();
                break;
            case 3:
                updateMemberInfo();
                break;
            case 4:
                deleteMember();
                break;
            case 5:
                searchMember();
                break;
            case 6:
                sortMembers();
                break;
            case 7:
                std::cout << std::string(50, '=') << "\n";
                std::cout << "|Logged out of system. Thank you and come again!|\n";
                std::cout << std::string(50, '=') << "\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void deleteMember() {
    int memberID;
    std::cout << "+" << std::string(45, '-') << "+";
    std::cout << "\n|" << std::string(15, '-') << " DELETE MEMBER " << std::string(15, '-') << "|\n";
    std::cout << "+" << std::string(45, '-') << "+\n";
    std::cout << "Enter Member ID to delete: ";
    std::cin >> memberID;

    MemberNode* prevMember = nullptr;
    MemberNode* currentMember = memberList;

    while (currentMember) {
        if (currentMember->memberID == memberID) {
            // Member found, delete the member
            if (prevMember) {
                // If the member to be deleted is not the first node
                prevMember->next = currentMember->next;
            } else {
                // If the member to be deleted is the first node
                memberList = currentMember->next;
            }

            // Remove the member from the unordered_map
            memberMap.erase(memberID);

            // Free the memory occupied by the member node
            delete currentMember;

            std::cout << "Member with ID " << memberID << " deleted successfully!\n";
            return;
        }

        // Move to the next node
        prevMember = currentMember;
        currentMember = currentMember->next;
    }

    std::cout << "Member not found with ID: " << memberID << "\n";
}

void searchMember() {
    int searchID;
    std::string searchName;

    std::cout << "+" << std::string(46, '-') << "+";
    std::cout << "\n|" << std::string(15, '-') << " MEMBER SEARCH " << std::string(16, '-') << "|\n";
    std::cout << "+" << std::string(46, '-') << "+\n";
    std::cout << "Enter Member ID to search: ";
    std::cin >> searchID;
    std::cin.ignore(); // Ignore the newline character

    MemberNode* currentMember = memberList;

    // Search by ID
    while (currentMember) {
        if (currentMember->memberID == searchID) {
            std::cout << "Member found!\n";
            std::cout << "Member ID: " << currentMember->memberID << "\n";
            std::cout << "Member Name: " << currentMember->memberName << "\n";
            std::cout << "Membership Type: " << currentMember->membershipType << "\n";
            std::cout << "Member Status: " << (currentMember->isMemberActive ? "Active" : "Inactive") << "\n";
            return;
        }
        currentMember = currentMember->next;
    }

    // If not found by ID, search by Name
    currentMember = memberList;
    std::cout << "Searching by Name...\n";
    while (currentMember) {
        if (currentMember->memberName == searchName) {
            std::cout << "Member found!\n";
            std::cout << "Member ID: " << currentMember->memberID << "\n";
            std::cout << "Member Name: " << currentMember->memberName << "\n";
            std::cout << "Membership Type: " << currentMember->membershipType << "\n";
            std::cout << "Member Status: " << (currentMember->isMemberActive ? "Active" : "Inactive") << "\n";
            return;
        }
        currentMember = currentMember->next;
    }

    // If not found by ID or Name
    std::cout << "Member not found with ID or Name: " << searchID << "/" << searchName << "\n";
}

void updateMemberInfo() {
    int memberID;
    std::cout << "+" << std::string(47, '-') << "+";
    std::cout << "\n+" << std::string(10, '-') << " UPDATE MEMBER INFORMATION " << std::string(10, '-') << "+\n";
    std::cout << "+" << std::string(47, '-') << "+\n";
    std::cout << "Enter Member ID: ";
    std::cin >> memberID;

    MemberNode* currentMember = memberList;
    while (currentMember) {
        if (currentMember->memberID == memberID) {
            // Member found, update information
            std::cout << "Enter new Member Name: ";
            std::cin.ignore();
            std::getline(std::cin, currentMember->memberName);

            std::cout << "Enter new Membership Type (P for Premium, N for Normal, S for Student): ";
            std::cin >> currentMember->membershipType;

            // Validate membership type
            if (currentMember->membershipType != 'P' && currentMember->membershipType != 'N' &&
                currentMember->membershipType != 'S') {
                std::cout << "Invalid membership type. Defaulting to Normal.\n";
                currentMember->membershipType = 'N';  // Default to Normal if invalid input
            }

            std::cout << "Enter new Member Status (1 for Active, 0 for Inactive): ";
            std::cin >> currentMember->isMemberActive;

            // Update information in member_credentials.txt
            updateMemberFile(currentMember);

            // Update information in the unordered_map
            memberMap[memberID] = {currentMember->memberName, currentMember->membershipType,
                                   currentMember->isMemberActive};

            std::cout << "Member information updated successfully!\n";
            return;
        }
        currentMember = currentMember->next;
    }

    std::cout << "Member not found with ID: " << memberID << "\n";
}

void updateMemberFile(MemberNode* member) {
    // Open the file in read mode to read existing content
    std::ifstream infile("member_credentials.txt");  // Fix: Add std:: before ifstream
    if (!infile.is_open()) {
        std::cout << "Error opening file for reading.\n";
        return;
    }

    // Open a temporary file to write modified content
    std::ofstream tempFile("temp_member_credentials.txt", std::ios::out);  // Fix: Add std:: before ofstream
    if (!tempFile.is_open()) {
        std::cout << "Error opening temporary file for writing.\n";
        infile.close();
        return;
    }

    int storedMemberID;
    std::string storedMemberName;
    bool storedIsMemberActive;

    // Read data from infile and write to tempFile, making modifications for the updated member
    while (infile >> storedMemberID >> storedMemberName >> storedIsMemberActive) {
        if (storedMemberID == member->memberID) {
            tempFile << member->memberID << " " << member->memberName << " " << member->isMemberActive << "\n";
        } else {
            tempFile << storedMemberID << " " << storedMemberName << " " << storedIsMemberActive << "\n";
        }
    }

    // Close both files
    infile.close();
    tempFile.close();

    // Rename the temporary file to the original file
    if (rename("temp_member_credentials.txt", "member_credentials.txt") != 0) {
        std::cout << "Error renaming file.\n";
    }
}

void createMemberAccount() {
    string newMemberName;
    char membershipType;
    bool isMemberActive = true;

    cout << "+" << string(49, '-') << "+";
    cout << "\n|" << string(15, '-') << " CREATE NEW MEMBER " << string(15, '-') << "|\n";
    cout << "+" << string(49, '-') << "+\n";
    cout << "Enter a new member name: ";
    cin.ignore();
    getline(cin, newMemberName);

    cout << "Enter membership type (P for Premium, N for Normal, S for Student): ";
    cin >> membershipType;

    // Validate membership type
    if (membershipType != 'P' && membershipType != 'N' && membershipType != 'S') {
        cout << "Invalid membership type. Defaulting to Normal.\n";
        membershipType = 'N';  // Default to Normal if invalid input
    }

    // Create a new member node
    MemberNode* newMember = new MemberNode;
    newMember->memberID = generateUniqueMemberID();
    newMember->memberName = newMemberName;
    newMember->isMemberActive = isMemberActive;

    // Set the membership type
    newMember->membershipType = membershipType;

    newMember->next = nullptr;

    // Add the new member to the linked list
    if (!memberList) {
        memberList = newMember;
    } else {
        MemberNode* currentMember = memberList;
        while (currentMember->next) {
            currentMember = currentMember->next;
        }
        currentMember->next = newMember;
    }

    // Add the new member to the unordered_map
    memberMap[newMember->memberID] = {newMemberName, membershipType, isMemberActive};

    // Save the new member information to the credentials file
    ofstream memberFile("member_credentials.txt", ios::app);
    if (memberFile.is_open()) {
        memberFile << newMember->memberID << " " << newMember->memberName << " " << newMember->membershipType << " "
                   << newMember->isMemberActive << "\n";
        memberFile.close();
        cout << "Member account created successfully!\n";
        cout << "New Member Name: " << newMemberName << "\n";
    } else {
        cout << "Error opening file for writing.\n";
    }
}

void createAdminAccount() {
    string newAdminUsername, newAdminPassword;


    cout << "\n\n+" << string(65, '-') << "+";
    cout << "\n|" << string(22, '-') << " CREATE ADMIN ACCOUNT " << string(21, '-') << "|\n";
    cout << "+" << string(65, '-') << "+\n";
    cout << "Enter a new admin username: ";
    cin >> newAdminUsername;
    cout << "Enter a new admin password: ";
    cin >> newAdminPassword;

    ofstream adminFile("admin_credentials.txt", ios::app);

    if (adminFile.is_open()) {
        adminFile << newAdminUsername << " " << newAdminPassword << "\n";
        adminFile.close();
        cout << "\n\n+" << string(36, '=') << "+";
        cout << "\n|" << " ADMIN ACCOUNT SUCCESFULLY CREATED " << "|\n";
        cout << "+" << string(36, '=') << "+\n";
        cout << "New Admin Username: " << newAdminUsername << "\n\n";
    } else {
        cout << "Error opening file for writing.\n";
    }
}

bool isValidMember(int enteredMemberID) {
    MemberNode* currentMember = memberList;

    while (currentMember) {
        if (currentMember->memberID == enteredMemberID) {
            return true; // Valid member ID
        }
        currentMember = currentMember->next;
    }

    return false; // Member ID not found
}

bool isValidAdmin(const string& enteredUsername, const string& enteredPassword) {
    string storedUsername, storedPassword;

    ifstream adminFile("admin_credentials.txt");

    if (adminFile.is_open()) {
        while (adminFile >> storedUsername >> storedPassword) {
            if (storedUsername == enteredUsername && storedPassword == enteredPassword) {
                adminFile.close();
                return true; // Valid admin credentials
            }
        }

        adminFile.close();
    }

    return false; // No match found
}

bool compareMembersByName(const MemberNode* a, const MemberNode* b) {
    string nameA = a->memberName;
    string nameB = b->memberName;

    // Convert names to lowercase for case-insensitive comparison
    transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
    transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);

    return nameA < nameB;
}

bool compareMembersByMembershipType(const MemberNode* a, const MemberNode* b) {
    return a->membershipType < b->membershipType;
}

void sortMembers() {
    // Create a vector to store the members for sorting
    vector<MemberNode*> membersVector;

    // Populate the vector with members from the linked list
    MemberNode* currentMember = memberList;
    while (currentMember) {
        membersVector.push_back(currentMember);
        currentMember = currentMember->next;
    }

    // Ask the user for the sorting criteria
    cout << "How would you like to sort members? (1. Name, 2. Membership Type): ";
    int sortingChoice;
    cin >> sortingChoice;

    // Sort the vector based on the chosen criteria using merge sort
    switch (sortingChoice) {
        case 1:
            // Sort based on member names (case-insensitive)
            mergeSort(membersVector, 0, membersVector.size() - 1);
            sort(membersVector.begin(), membersVector.end(), compareMembersByName);
            break;
        case 2:
            // Sort based on membership type
            mergeSort(membersVector, 0, membersVector.size() - 1);
            sort(membersVector.begin(), membersVector.end(), compareMembersByMembershipType);
            break;
        default:
            cout << "Invalid choice. Sorting by membership type by default.\n";
            // Sort based on membership type in case of an invalid choice
            mergeSort(membersVector, 0, membersVector.size() - 1);
            sort(membersVector.begin(), membersVector.end(), compareMembersByMembershipType);
    }

    // Display the sorted members
    cout << "+" << string(47, '-') << "+";
    cout << "\n|" << string(15, '-') << " SORTED MEMBERS " << string(16, '-') << "|\n";
    cout << "+" << string(47, '-') << "+\n";
    for (const auto& member : membersVector) {
        cout << "Member ID: " << member->memberID << "\n";
        cout << "Member Name: " << member->memberName << "\n";
        cout << "Membership Type: " << member->membershipType << "\n";
        cout << "Member Status: " << (member->isMemberActive ? "Active" : "Inactive") << "\n";
        cout << "---------------------------\n";
    }
}

void merge(vector<MemberNode*>& membersVector, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays to hold left and right halves
    vector<MemberNode*> leftArray(n1);
    vector<MemberNode*> rightArray(n2);

    // Copy data to temporary arrays leftArray[] and rightArray[]
    for (int i = 0; i < n1; i++)
        leftArray[i] = membersVector[left + i];
    for (int j = 0; j < n2; j++)
        rightArray[j] = membersVector[mid + 1 + j];

    // Merge the temporary arrays back into membersVector[left..right]
    int i = 0;  // Initial index of left subarray
    int j = 0;  // Initial index of right subarray
    int k = left;  // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (compareMembersByMembershipType(leftArray[i], rightArray[j])) {
            membersVector[k] = leftArray[i];
            i++;
        } else {
            membersVector[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArray[], if there are any
    while (i < n1) {
        membersVector[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArray[], if there are any
    while (j < n2) {
        membersVector[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(vector<MemberNode*>& membersVector, int left, int right) {
    if (left < right) {
        // Same as (left + right) / 2, but avoids overflow for large left and right
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(membersVector, left, mid);
        mergeSort(membersVector, mid + 1, right);

        // Merge the sorted halves
        merge(membersVector, left, mid, right);
    }
}

int main() {
    loadMembersFromFile();

    int adminChoice;

    do {
        displayMainMenu();
        cout << "Enter your choice: ";

        while (!(cin >> adminChoice)) {
            cout << "Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (adminChoice) {
            case 1:
                adminLogin();
                break;
            case 2:
                createAdminAccount();
                break;
            case 3:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (adminChoice != 3);

    return 0;
}
