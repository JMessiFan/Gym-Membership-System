#ifndef GYM_MEMBERSHIP_H
#define GYM_MEMBERSHIP_H

#include <string>
#include <unordered_map>
#include <vector>

struct MemberNode {
    int memberID;
    std::string memberName;
    char membershipType;
    bool isMemberActive;
    MemberNode* next;
};

struct Member {
    std::string memberName;
    char membershipType;
    bool isMemberActive;
    // You can add more member-related information here
};

struct Admin {
    std::string adminUsername;
    std::string adminPassword;
};

// Function prototypes
void displayMainMenu();
void adminActionsMenu();
void adminLogin();
void memberLogin();
void createAdminAccount();
void createMemberAccount();
bool isValidAdmin(const std::string& username, const std::string& password);
bool isValidAdmin(const std::string& enteredUsername, const std::string& enteredPassword);
bool isValidMember(int enteredMemberID);
void updateMemberInfo();
void updateMemberFile(MemberNode* member);
void loadMembersFromFile();
void deleteMember();
void searchMember();
bool compareMembersByMembershipType(const MemberNode* a, const MemberNode* b);
bool compareMembersByName(const MemberNode* a, const MemberNode* b);
void sortMembers();
int generateUniqueMemberID();
void displayMembersInfo();
void mergeSort(std::vector<MemberNode*>& membersVector, int left, int right);
void merge(std::vector<MemberNode*>& membersVector, int left, int mid, int right);

#endif // GYM_MEMBERSHIP_H
