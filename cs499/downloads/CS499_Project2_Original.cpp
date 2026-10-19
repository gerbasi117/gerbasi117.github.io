#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct Client
{
    string name;
    int serviceChoice;
};

void clearBadInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedInteger(const string& prompt, int minValue, int maxValue)
{
    int value;

    while (true)
    {
        cout << prompt;

        if (cin >> value && value >= minValue && value <= maxValue)
        {
            return value;
        }

        // Vulnerability identified: The original code accepted input without checking
        // whether it was numeric or within a valid range.
        // Fix: This code validates the input range and clears bad input before retrying.
        cout << "Invalid input. Please enter a number from "
             << minValue << " to " << maxValue << "." << endl;
        clearBadInput();
    }
}

unsigned long long hashPassword(const string& password)
{
    unsigned long long hash = 1469598103934665603ULL;

    for (char c : password)
    {
        hash ^= static_cast<unsigned char>(c);
        hash *= 1099511628211ULL;
    }

    return hash;
}

bool CheckUserPermissionAccess()
{
    const string allowedUsername = "Brandon";

    // Vulnerability identified: The Project One version used a hardcoded plaintext
    // password directly in the password comparison.
    // Fix: This version compares a hashed password value instead of comparing the
    // entered password directly to a plaintext password string.
    const unsigned long long expectedPasswordHash = 4591456448132287629ULL; // hash for "123"

    const int maxAttempts = 3;

    for (int attempt = 1; attempt <= maxAttempts; attempt++)
    {
        string username;
        string password;

        cout << "Enter your username: ";
        cin >> username;

        cout << "Enter your password: ";
        cin >> password;

        // Vulnerability identified: The Project One code asked for a username, but the
        // username was not actually validated.
        // Fix: This code checks both the username and the password hash before allowing access.
        if (username == allowedUsername && hashPassword(password) == expectedPasswordHash)
        {
            return true;
        }

        cout << "Invalid username or password. Attempts remaining: "
             << (maxAttempts - attempt) << endl;
    }

    // Vulnerability identified: The Project One code allowed unlimited login attempts.
    // Fix: This version limits the user to three attempts before denying access.
    return false;
}

string serviceName(int serviceChoice)
{
    if (serviceChoice == 1)
    {
        return "Brokerage";
    }

    if (serviceChoice == 2)
    {
        return "Retirement";
    }

    return "Unknown";
}

void DisplayInfo(const vector<Client>& clients)
{
    cout << "  Client's Name    Service Selected (1 = Brokerage, 2 = Retirement)" << endl;

    for (size_t i = 0; i < clients.size(); i++)
    {
        cout << (i + 1) << ". "
             << clients[i].name
             << " selected option "
             << clients[i].serviceChoice
             << " (" << serviceName(clients[i].serviceChoice) << ")"
             << endl;
    }
}

void ChangeCustomerChoice(vector<Client>& clients)
{
    int clientNumber = getValidatedInteger(
        "Enter the number of the client that you wish to change: ",
        1,
        static_cast<int>(clients.size())
    );

    int newService = getValidatedInteger(
        "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement): ",
        1,
        2
    );

    // Vulnerability identified: The Project One code allowed any client number and any
    // service value to be entered.
    // Fix: This version validates that the selected client exists and that the new
    // service choice is only 1 or 2 before making the update.
    clients[clientNumber - 1].serviceChoice = newService;

    cout << "Client service choice updated." << endl;
}

int main()
{
    // Vulnerability identified: The Project One program relied on several global variables
    // for client names, service choices, menu choice, and login result.
    // Fix: This version keeps client information in a local vector inside main and passes it
    // to functions only when needed.
    vector<Client> clients = {
        {"Bob Jones", 1},
        {"Sarah Davis", 2},
        {"Amy Friendly", 1},
        {"Johnny Smith", 1},
        {"Carol Spears", 2}
    };

    cout << "Project Two secure version created by Brandon Gerbasi" << endl;
    cout << "Hello! Welcome to our Investment Company" << endl;

    if (!CheckUserPermissionAccess())
    {
        cout << "Access denied. Too many failed login attempts." << endl;
        return 1;
    }

    int choice = 0;

    while (choice != 3)
    {
        cout << "What would you like to do?" << endl;
        cout << "DISPLAY the client list (enter 1)" << endl;
        cout << "CHANGE a client's choice (enter 2)" << endl;
        cout << "Exit the program.. (enter 3)" << endl;

        // Vulnerability identified: The Project One code read the menu choice directly
        // without checking for non-numeric input or invalid menu numbers.
        // Fix: This version validates that the user enters only 1, 2, or 3.
        choice = getValidatedInteger("Enter your choice: ", 1, 3);

        cout << "You chose " << choice << endl;

        if (choice == 1)
        {
            DisplayInfo(clients);
        }
        else if (choice == 2)
        {
            ChangeCustomerChoice(clients);
        }
        else if (choice == 3)
        {
            cout << "Exiting program." << endl;
        }
    }

    return 0;
}
