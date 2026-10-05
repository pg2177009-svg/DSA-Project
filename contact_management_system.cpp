#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// Structure representing one contact
struct Contact {
    int id;
    string name;
    string phone;
    string email;
};

// -------------------- Utility Functions --------------------

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string toLowerCase(string text) {
    for (char &ch : text) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

bool isValidPhone(const string &phone) {
    if (phone.empty()) return false;

    int digits = 0;
    for (char ch : phone) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            digits++;
        } else if (ch != '+' && ch != '-' && ch != ' ' && ch != '(' && ch != ')') {
            return false;
        }
    }

    return digits >= 7;
}

bool isValidEmail(const string &email) {
    size_t at = email.find('@');
    size_t dot = email.find('.', at == string::npos ? 0 : at);

    return at != string::npos &&
           dot != string::npos &&
           at > 0 &&
           dot > at + 1 &&
           dot < email.length() - 1;
}

int getNextId(const vector<Contact>& contacts) {
    int maxId = 0;

    for (const Contact &c : contacts) {
        if (c.id > maxId) {
            maxId = c.id;
        }
    }

    return maxId + 1;
}

int findContactIndexById(const vector<Contact>& contacts, int id) {
    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].id == id) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

bool isDuplicatePhone(const vector<Contact>& contacts,
                      const string& phone,
                      int ignoreId = -1) {
    for (const Contact &c : contacts) {
        if (c.id != ignoreId && c.phone == phone) {
            return true;
        }
    }

    return false;
}

// -------------------- Display Functions --------------------

void displayContact(const Contact& c) {
    cout << "\n----------------------------------------\n";
    cout << "ID    : " << c.id << '\n';
    cout << "Name  : " << c.name << '\n';
    cout << "Phone : " << c.phone << '\n';
    cout << "Email : " << c.email << '\n';
    cout << "----------------------------------------\n";
}

void displayAllContacts(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts available.\n";
        return;
    }

    cout << "\n========== ALL CONTACTS ==========\n";

    for (const Contact &c : contacts) {
        displayContact(c);
    }
}

// -------------------- Main Operations --------------------

void addContact(vector<Contact>& contacts) {
    Contact newContact;
    newContact.id = getNextId(contacts);

    cout << "\n========== ADD CONTACT ==========\n";

    do {
        cout << "Enter name: ";
        getline(cin, newContact.name);

        if (newContact.name.empty()) {
            cout << "Name cannot be empty.\n";
        }
    } while (newContact.name.empty());

    do {
        cout << "Enter phone number: ";
        getline(cin, newContact.phone);

        if (!isValidPhone(newContact.phone)) {
            cout << "Invalid phone number. Enter at least 7 digits.\n";
        } else if (isDuplicatePhone(contacts, newContact.phone)) {
            cout << "This phone number already exists.\n";
            newContact.phone.clear();
        }
    } while (!isValidPhone(newContact.phone));

    do {
        cout << "Enter email: ";
        getline(cin, newContact.email);

        if (!isValidEmail(newContact.email)) {
            cout << "Invalid email format.\n";
        }
    } while (!isValidEmail(newContact.email));

    contacts.push_back(newContact);

    cout << "\nContact added successfully. Contact ID = "
         << newContact.id << "\n";
}

void deleteContact(vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts to delete.\n";
        return;
    }

    int id;
    cout << "\nEnter Contact ID to delete: ";

    if (!(cin >> id)) {
        clearInput();
        cout << "Invalid ID.\n";
        return;
    }
    clearInput();

    int index = findContactIndexById(contacts, id);

    if (index == -1) {
        cout << "Contact not found.\n";
        return;
    }

    displayContact(contacts[index]);

    char confirm;
    cout << "Delete this contact? (y/n): ";
    cin >> confirm;
    clearInput();

    if (tolower(static_cast<unsigned char>(confirm)) == 'y') {
        contacts.erase(contacts.begin() + index);
        cout << "Contact deleted successfully.\n";
    } else {
        cout << "Delete operation cancelled.\n";
    }
}

void searchByName(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts available.\n";
        return;
    }

    string query;
    cout << "\nEnter name to search: ";
    getline(cin, query);

    if (query.empty()) {
        cout << "Search name cannot be empty.\n";
        return;
    }

    string lowerQuery = toLowerCase(query);
    bool found = false;

    cout << "\n========== NAME SEARCH RESULTS ==========\n";

    for (const Contact &c : contacts) {
        if (toLowerCase(c.name).find(lowerQuery) != string::npos) {
            displayContact(c);
            found = true;
        }
    }

    if (!found) {
        cout << "No contact found with that name.\n";
    }
}

void searchByPhone(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts available.\n";
        return;
    }

    string query;
    cout << "\nEnter phone number to search: ";
    getline(cin, query);

    if (query.empty()) {
        cout << "Phone number cannot be empty.\n";
        return;
    }

    bool found = false;

    cout << "\n========== PHONE SEARCH RESULTS ==========\n";

    for (const Contact &c : contacts) {
        if (c.phone == query) {
            displayContact(c);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No contact found with that phone number.\n";
    }
}

void updateContact(vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts available to update.\n";
        return;
    }

    int id;
    cout << "\nEnter Contact ID to update: ";

    if (!(cin >> id)) {
        clearInput();
        cout << "Invalid ID.\n";
        return;
    }
    clearInput();

    int index = findContactIndexById(contacts, id);

    if (index == -1) {
        cout << "Contact not found.\n";
        return;
    }

    Contact &c = contacts[index];

    cout << "\nCurrent details:";
    displayContact(c);

    cout << "\nEnter new name (press Enter to keep current): ";
    string input;
    getline(cin, input);

    if (!input.empty()) {
        c.name = input;
    }

    while (true) {
        cout << "Enter new phone (press Enter to keep current): ";
        getline(cin, input);

        if (input.empty()) {
            break;
        }

        if (!isValidPhone(input)) {
            cout << "Invalid phone number.\n";
        } else if (isDuplicatePhone(contacts, input, c.id)) {
            cout << "This phone number belongs to another contact.\n";
        } else {
            c.phone = input;
            break;
        }
    }

    while (true) {
        cout << "Enter new email (press Enter to keep current): ";
        getline(cin, input);

        if (input.empty()) {
            break;
        }

        if (!isValidEmail(input)) {
            cout << "Invalid email format.\n";
        } else {
            c.email = input;
            break;
        }
    }

    cout << "\nContact updated successfully.\n";
}

void sortContactsAlphabetically(vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "\nNo contacts available to sort.\n";
        return;
    }

    sort(contacts.begin(), contacts.end(),
         [](const Contact& a, const Contact& b) {
             return toLowerCase(a.name) < toLowerCase(b.name);
         });

    cout << "\nContacts sorted alphabetically by name.\n";
    displayAllContacts(contacts);
}

// -------------------- Menu --------------------

void showMenu() {
    cout << "\n\n========================================\n";
    cout << "       CONTACT MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add Contact\n";
    cout << "2. Delete Contact\n";
    cout << "3. Search by Name\n";
    cout << "4. Search by Phone Number\n";
    cout << "5. Update Contact Details\n";
    cout << "6. Display All Contacts\n";
    cout << "7. Sort Contacts Alphabetically\n";
    cout << "8. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<Contact> contacts;

    // Sample records can be uncommented for demonstration.
    // contacts.push_back({1, "Rahul", "9876543210", "rahul@gmail.com"});
    // contacts.push_back({2, "Ananya", "9123456780", "ananya@gmail.com"});

    int choice;

    cout << "\nWelcome to the Contact Management System!\n";

    do {
        showMenu();

        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid choice. Please enter a number from 1 to 8.\n";
            continue;
        }

        clearInput();

        switch (choice) {
            case 1:
                addContact(contacts);
                break;

            case 2:
                deleteContact(contacts);
                break;

            case 3:
                searchByName(contacts);
                break;

            case 4:
                searchByPhone(contacts);
                break;

            case 5:
                updateContact(contacts);
                break;

            case 6:
                displayAllContacts(contacts);
                break;

            case 7:
                sortContactsAlphabetically(contacts);
                break;

            case 8:
                cout << "\nThank you for using Contact Management System.\n";
                break;

            default:
                cout << "Invalid choice. Please choose 1 to 8.\n";
        }

    } while (choice != 8);

    return 0;
}
