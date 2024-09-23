#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex> // validation
#include <vector>

using namespace std;

void signup();
void login();
void resetPassword();
bool isValidEmail(const string &userEmail);
bool checkUserExists(const string &userEmail);
string getPasswordForEmail(const string &userEmail);
bool checkPasswordLength(const string &userPassword);

// Email Validation
bool isValidEmail(const string &userEmail)
{
    // Regular expression
    const regex pattern(R"((\w+)(\.{0,1}(\w+))*@(\w+)(\.(\w+))+)");
    return regex_match(userEmail, pattern);
}

string getPasswordForEmail(const string &userEmail)
{
    ifstream infile("users_data.dat");
    string line;
    string storeEmail;
    string storePass;

    /*
istringstream iss(line); creates a stream from the string `line`, allowing you to extract individual data elements (e.g., email and password) using the stream extraction operator (`>>`).
*/
    while (getline(infile, line))
    {
        istringstream iss(line);
        iss >> storeEmail >> storePass;
        if (storeEmail == userEmail)
        {
            return storePass;
        }
    }
    return "";
}

bool checkUserExists(const string &userEmail)
{
    ifstream infile("users_data.dat");
    string line;

    while (getline(infile, line))
    {
        if (line.find(userEmail) != string::npos)
        {
            return true;
        }
    }
    return false;
}

bool checkPasswordLength(const string &userPassword)
{
    int passMin = 8;
    int passMax = 16;

    if (userPassword.length() < passMin || userPassword.length() > passMax)
    {
        cout << "The password must be between " << passMin << " and " << passMax << " characters." << endl;
        return false;
    }
    return true;
}

void signup()
{

    string userEmail;
    string userPassword;
    string userConfirmPassword;

    int passMin = 8;
    int passMax = 16;

    cout << "Welcome to sign up page." << endl;

    while (true)
    {

        cout << "Enter email: ";
        cin >> userEmail;
        if (!isValidEmail(userEmail))
        {
            cout << "Please enter a valid email address." << endl;
            continue;
        }
        if (checkUserExists(userEmail))
        {
            cout << "Email already exists. Please login." << endl;
        }
        break;
    }

    while (true)
    {

        cout << "Enter password: ";
        cin >> userPassword;

        if (!checkPasswordLength(userPassword))
        {
            continue;
        }

        checkPasswordLength(userPassword);

        cout << "Enter confirm password: ";
        cin >> userConfirmPassword;

        if (userPassword != userConfirmPassword)
        {

            cout << "Failed to create your account." << endl;
            continue;
        }
        else
        {
            cout << "Your account has been created successfully!" << endl;
            break;
        }
    }

    // Store data
    ofstream outfile("users_data.dat", ios::app);
    outfile << userEmail << " " << userPassword << endl;
    outfile.close();
}

void login()
{
    string userEmail;
    string userPassword;

    cout << "Welcome to login page." << endl;

    cout << "Enter email: ";
    cin >> userEmail;

    cout << "Enter password: ";
    cin >> userPassword;

    if (checkUserExists(userEmail))
    {
        string storedPassword;
        storedPassword = getPasswordForEmail(userEmail);
        if (storedPassword == userPassword)
        {
            cout << "Login successful!" << endl;
        }
        else
        {
            cout << "Invalid password. Please try again." << endl;
        }
    }
    else
    {
        cout << "Email not found. Please register first." << endl;
    }
}

void resetPassword()
{
    string userEmail;
    string newPass;
    string confirmPass;
    bool userFound = false;

    cout << "Password Reset" << endl;
    cout << "Enter your email: ";
    cin >> userEmail;

    if (!checkUserExists(userEmail))
    {
        cout << "Email not found. Please register first." << endl;
        return;
    }

    while (true)
    {
        cout << "Enter new password: ";
        cin >> newPass;

        if (!checkPasswordLength(newPass))
        {
            continue;
        }

        cout << "Confirm new password: ";
        cin >> confirmPass;

        if (newPass != confirmPass)
        {
            cout << "Passwords do not match. Try again." << endl;
            continue;
        }
        break;
    }

    ifstream infile("users_data.dat");
    ofstream tempFile("temp.dat");
    string line;
    string email;
    string password;

    while (getline(infile, line))
    {
        istringstream iss(line);
        iss >> email >> password;

        if (email == userEmail)
        {
            tempFile << email << " " << newPass <<endl;; // Write new password
            userFound = true;
        }
    }

    infile.close();
    tempFile.close();

    // Replace original file with updated file
    remove("users_data.dat");
    rename("temp.dat", "users_data.dat");

    if (userFound)
    {
        cout << "Password reset successfully!" << endl;
    }
}

int main()
{

    int userInput;
    bool isRunning = true;

    cout << "---------------------------------------------" << endl;
    cout << "|      Login and Registeration System       |" << endl;
    cout << "---------------------------------------------" << endl;

    while (isRunning)
    {

        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Forgot Password" << endl;
        cout << "4. Exit" << endl;

        cout << "Select a valid option: ";
        cin >> userInput;

        switch (userInput)
        {

        case 1:
            signup();

            break;
        case 2:
            login();

            break;
        case 3:
            resetPassword();

            break;
        case 4:
            cout << "Thank you...!" << endl;

            isRunning = false;

            break;

        default:
            cout << "Please provide a valid selection. " << endl;
        }
    };

    return 0;
}