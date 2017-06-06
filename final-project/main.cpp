//============================================================================
// Name        : Bank Simulation Program
// Author      : Kirby James
//============================================================================

/*******\
* FINAL *
\*******/

/************************ Pseudo Algorithm *****************************
 Functions:
 function A - Add new customer to the current bank.
 function R - Remove customer from the current bank.
 function P - Print all customers in the current bank to the monitor.
 function W - Make a withdraw.
 function D - Make a deposit.
 function S - Save current bank information to a file.
 function T - Sort list.
 function N - Sort on last names.
 function C - Sort on city.
 function B - Sort on balance.
 function V - Print a customer√s details in a pag`e format to the monitor.
 function X - Exit the program.

 Extra Credit:
 1) Add a search function that is not case sensitive and/or partial name search. (10 Pts.)
 a. Make the search partial search, so if part of the name is given locate all names that contain the
 sub string. (10 more points)
 2) Update and save the input file before stopping the program. Prompt user for a new file name. (10
 Pts.)
 3) Develop a structure to represent a Bank (a sample form of the structure): (20 Pts.)
 Name (Bank name)
 List of accounts (Array of Customer type, assume size 50) Total Assets (Assets of the bank is total of all the deposits) + all the necessary utility functions (read, print, process,...)
 4) Add any features that may improve the program. (Email me first to make sure it is worth extra credit)
 ************************ End of Algorithm *****************************/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#define MAX 50
using namespace std;

struct Accounts {
    // TODO:
};

struct Bank {
    string bankName;
    double totalAssets = 0;
    // TODO:
};

struct Customer {
    string fullName, streetAddress, city, state;
    double balance;
};

struct Name {
    bool existingName = false;
    int index;
};

// Prototypes:
bool checkEmptyFile(fstream &fin);
void promptUserFile(string &iFileName);
void readInfo(fstream &fin, Bank &BankInfo, Customer Info[]);
void validateFileName(fstream &fin, Bank &BankInfo, Customer Info[], string iFileName);
int countCustomer(Customer Info[]);
void pauseCls();
void addCustomer(Customer Info[], Bank &BankInfo, int counter);
void removeCustomer(Customer Info[], Bank &BankInfo);
void displayTableFormat(Customer Info[], Bank &BankInfo);
Name checkCustomerName(string acctName, Customer Info[]);
void withdraw(Customer Info[], Bank &BankInfo);
void deposit(Customer Info[], Bank &BankInfo);
void viewCustomerDetails(Customer Info[]);
void saveBankInfoToFile(Customer Info[], Bank &BankInfo);
void saveUpdateInputFile(Customer Info[], Bank &BankInfo);
void sorting(Customer Info[], int j);
void bubbleSort(Customer Info[], char o);
void sortCustomers(Customer Info[]);
void showMenu(char menu[]);
void chooseMenu(Customer Info[], Bank &BankInfo);

// Functions:

int main()
{
    string iFileName;
    Customer Info[MAX];
    Bank BankInfo;
    fstream fin;

    promptUserFile(iFileName);
    validateFileName(fin, BankInfo, Info, iFileName);
    pauseCls();
    chooseMenu(Info, BankInfo);

    return 0;
}

// check file if it is empty
bool checkEmptyFile(fstream &fin)
{
    return fin.peek() == std::ifstream::traits_type::eof();
}

// prompts user for the file name
void promptUserFile(string &iFileName)
{
    cout << "Welcome, how may I help you today?" << endl;
    cout << "Please enter input file name: ";
    getline(cin, iFileName);
    cout << endl;
}

void readInfo(fstream &fin, Bank &BankInfo, Customer Info[])
{
    int counter = 0;
    while (fin.good()) {
        if (counter == 0) {
            getline(fin, BankInfo.bankName);
        }
        getline(fin, Info[counter].fullName);
        getline(fin, Info[counter].streetAddress);
        getline(fin, Info[counter].city);
        fin >> Info[counter].state;
        fin >> Info[counter].balance;
        fin.ignore();

        BankInfo.totalAssets += Info[counter].balance;
        counter++;
    }
    //output the total number of customer/s added to the bank from the input file
    cout << "Successfully added " << counter << " customers to Bank: " << BankInfo.bankName;

    //close the file
    fin.close();
}

// validates the input file name
void validateFileName(fstream &fin, Bank &BankInfo, Customer Info[], string iFileName)
{
    // open the file name
    fin.open(iFileName.c_str());

    if (fin && !checkEmptyFile(fin)) {
        readInfo(fin, BankInfo, Info);
    }
    else if (checkEmptyFile(fin)) {
        cout << "File is empty!";

        exit(0);
    }
    else if (!fin.eof()) {
        // If end-of-file not reached
        cout << "Error reading the file" << endl;

        exit(0);
    }
    else {
        // if file couldn't open
        cout << "Couldn't read the file" << endl;

        exit(0);
    }
}

// counts how many customer
int countCustomer(Customer Info[])
{
    int count = 0;
    for (int x = 0; x < MAX; x++) {
        if (Info[x].fullName != "")
            count++;
    }
    return count;
}

// creates a system pause every after a function that has been done
void pauseCls() {
    cout << endl << endl;
    system("pause");
    system("cls");
}

// add customer to the current bank
void addCustomer(Customer Info[], Bank &BankInfo)
{

    cin.ignore();
    int i = countCustomer(Info);
    cout << "ADD NEW CUSTOMER TO THE CURRENT BANK: " << BankInfo.bankName << endl;
    cout << "Please enter full customer name: ";
    getline(cin, Info[i].fullName);
    cout << "Please enter street address: ";
    getline(cin, Info[i].streetAddress);
    cout << "Please enter city: ";
    getline(cin, Info[i].city);
    cout << "Please enter state: ";
    cin >> Info[i].state;
    cout << "Please enter balance: ";
    cin >> Info[i].balance;
    cout << endl;

    BankInfo.totalAssets += Info[i].balance;
    cout << "Successfully added new customer to the current Bank" << endl;

    pauseCls();

}

// remove a specif customer from the current bank
void removeCustomer(Customer Info[], Bank &BankInfo)
{
    string acctName;

    cout << "REMOVE CUSTOMER FROM THE CURRENT BANK: " << BankInfo.bankName << endl;
    cout << "Please enter Account Name: ";
    cin.ignore();
    getline(cin, acctName);

    int acctId = checkCustomerName(acctName, Info).index;
    int i = countCustomer(Info);

    if (checkCustomerName(acctName, Info).existingName) {
        BankInfo.totalAssets -= Info[acctId].balance;
        for (int x = acctId; x < i; ++x) {
            Info[x] = Info[x + 1];
        }
        cout << "Successfully removed customer from the current Bank" << endl;
    }
    else {
        cout << "Account name doesn't exist!";
    }
    pauseCls();
}

// display in a table format
void displayTableFormat(Customer Info[], Bank &BankInfo)
{
    int i = countCustomer(Info);

    cout << BankInfo.bankName << " Report" << endl;
    cout << "Name \t\tBalance" << endl;
    cout << "--------------------------------" << endl;
    for (int x = 0; x < i; x++) {
        cout << setprecision(2) << fixed;
        cout << Info[x].fullName << "\t" << "$" << Info[x].balance << endl;
    }
    cout << "--------------------------------" << endl;
    cout << "Total Assets \t" << "$" << BankInfo.totalAssets;

    pauseCls();
}

// check if it is an existing customer in the Bank
Name checkCustomerName(string acctName, Customer Info[])
{
    Name cName;
    int i = countCustomer(Info);

    for (int x = 0; x < i; x++) {
        if (acctName == Info[x].fullName) {
            cName.existingName = true;
            cName.index = x;
        }
    }
    return cName;
}

// withdraw a specific amount of money from a chosen account
void withdraw(Customer Info[], Bank &BankInfo)
{
    string acctName;

    cout << "WITHDRAW FROM ACCOUNT" << endl;
    cout << "Please enter Account Name: ";
    cin.ignore();
    getline(cin, acctName);

    int acctId = checkCustomerName(acctName, Info).index;

    if (!checkCustomerName(acctName, Info).existingName) {
        cout << "Account name doesn't exist!" << endl;
    }
    else {
        double withdrawAmount = 0;
        cout << "Account Name: \t" << Info[acctId].fullName << endl;
        cout << setprecision(2) << fixed;
        cout << "Available Balance: \t" << "$" << Info[acctId].balance << endl;
        cout << "Withdraw Amount: ";
        cin >> withdrawAmount;
        if (withdrawAmount > Info[acctId].balance) {
            cout << "Insufficient funds.";
        }
        else {
            Info[acctId].balance = Info[acctId].balance - withdrawAmount;
            BankInfo.totalAssets -= withdrawAmount;
            cout << endl;
            cout << "Account Name: \t" << Info[acctId].fullName << endl;
            cout << "New Balance: \t" << "$" << Info[acctId].balance << endl;
        }
    }

    pauseCls();
}

// deposit a specific amount of money from a chosen account
void deposit(Customer Info[], Bank &BankInfo)
{
    string acctName;

    cout << "DEPOSIT TO ACCOUNT" << endl;
    cout << "Please enter Account Name: ";
    cin.ignore();
    getline(cin, acctName);

    int acctId = checkCustomerName(acctName, Info).index;

    if (!checkCustomerName(acctName, Info).existingName) {
        cout << "Account name doesn't exist!" << endl;
    }
    else {
        double depositAmount = 0;
        cout << "Account Name: \t" << Info[acctId].fullName << endl;
        cout << setprecision(2) << fixed;
        cout << "Available Balance: \t" << "$" << Info[acctId].balance << endl;
        cout << "Deposit Amount: ";
        cin >> depositAmount;
        if (depositAmount < 0) {
            cout << "Invalid amount.";
        }
        else {
            Info[acctId].balance = Info[acctId].balance + depositAmount;
            BankInfo.totalAssets += depositAmount;
            cout << endl;
            cout << "Account Name: \t" << Info[acctId].fullName << endl;
            cout << "New Balance: \t" << "$" << Info[acctId].balance << endl;
        }
    }
    pauseCls();
}

// view a specifc customer and prints it in a table format
void viewCustomerDetails(Customer Info[])
{
    string acctName;

    cout << "VIEW ACCOUNT DETAILS" << endl;
    cout << "Please enter Account Name: ";
    cin.ignore();
    getline(cin, acctName);

    int acctId = checkCustomerName(acctName, Info).index;

    if (checkCustomerName(acctName, Info).existingName) {
        cout << "Name\t: " << Info[acctId].fullName << endl;
        cout << "Address\t: " << Info[acctId].streetAddress << endl;
        cout << "City\t: " << Info[acctId].city << endl;
        cout << "State\t: " << Info[acctId].state << endl;
        cout << setprecision(2) << fixed;
        cout << "Balance\t: " << "$" << Info[acctId].balance;
    }
    else {
        cout << "Account name doesn't exist!";
    }
    pauseCls();
}


void saveBankInfoToFile(Customer Info[], Bank &BankInfo)
{
    ofstream fout;
    string oFileName;

    cout << "Please enter output file name: ";
    cin >> oFileName;
    fout.open(oFileName.c_str());

    //ADD: Check if file name is valid; FIX: Alignment
    if (fout) {
        int i = countCustomer(Info);
        fout << BankInfo.bankName << " Report" << endl;
        fout << "Name\t Address\t City\t State\t Balance" << endl;
        fout << "--------------------------------------------------------------------------------------------------" << endl;
        for (int x = 0; x < i; x++) {
            fout << setprecision(2) << fixed;
            fout << Info[x].fullName << "\t" << Info[x].streetAddress << "\t" << Info[x].city << "\t" << Info[x].state << "\t" << "$" << Info[x].balance << endl;
        }
        fout << "--------------------------------------------------------------------------------------------------" << endl;
        fout << "Total Assets $" << BankInfo.totalAssets;
    }

    pauseCls();
}

//2) Update and save the input file before stopping the program. Prompt user for a new file name. (10Pts.)
void saveUpdateInputFile(Customer Info[], Bank &BankInfo)
{
    ofstream fout;
    string fileName;

    cout << "SAVE/UPDATE INPUT FILE" << endl;
    cout << "Please enter file name: ";
    cin >> fileName;
    fout.open(fileName.c_str());

    if (fout) {
        int i = countCustomer(Info);
        fout << BankInfo.bankName;
        for (int x = 0; x < i; x++) {
            fout << endl;
            fout << Info[x].fullName << endl;
            fout << Info[x].streetAddress << endl;
            fout << Info[x].city << endl;
            fout << Info[x].state << endl;
            fout << Info[x].balance;
        }
        cout << "Successfully saved Bank Info into file." << endl;
        cout << "Goodbye World...";
    }
}

void sorting(Customer Info[], int j)
{
    string temp;
    temp = Info[j].fullName;
    Info[j].fullName = Info[j + 1].fullName;
    Info[j + 1].fullName = temp;

    temp = Info[j].city;
    Info[j].city = Info[j + 1].city;
    Info[j + 1].city = temp;

    temp = Info[j].streetAddress;
    Info[j].streetAddress = Info[j + 1].streetAddress;
    Info[j + 1].streetAddress = temp;

    temp = Info[j].state;
    Info[j].state = Info[j + 1].state;
    Info[j + 1].state = temp;

    double temp1;
    temp1 = Info[j].balance;
    Info[j].balance = Info[j + 1].balance;
    Info[j + 1].balance = temp1;
}

// sort customers depending on the option chosen from function sortCustomers()
void bubbleSort(Customer Info[], char o)
{
    int i = countCustomer(Info);

    for (int x = 1; x < i; x++)
    {
        for (int j = 0; j < i - 1; j++)
        {
            //Sort on Last names
            if ((Info[j].fullName > Info[j + 1].fullName) && o == 'N')
            {
                sorting(Info, j);
            }
            //Sort on City
            if ((Info[j].city > Info[j + 1].city) && o == 'C')
            {
                sorting(Info, j);
            }
            //Sort on Balances
            if ((Info[j].balance > Info[j + 1].balance) && o == 'B')
            {
                sorting(Info, j);
            }
        }
    }
}

// allows user to sort customers from the chosen option
void sortCustomers(Customer Info[])
{
    char option[2];

    cout << "SORT LIST" << endl;
    cout << "'N' -- Sort on Last names." << endl;
    cout << "'C' -- Sort on City." << endl;
    cout << "'B' -- Sort on Balance." << endl;

    cout << "Choose a letter from the menu: " << flush;
    cin >> option;

    if (option[1] != NULL) {
        cout << "Please try again." << endl;
    }
    else {
        char o = toupper(option[0]);
        if (o == 'N' || o == 'C' || o == 'B') {
            bubbleSort(Info, o);
        }
        else {
            cout << "Please choose a letter from the menu." << endl;
        }
    }
    pauseCls();
}


void showMenu(char menu[])
{
    cout << endl << "Menu:" << endl;
    cout << "'A' -- Add new customer to the current bank." << endl;
    cout << "'R' -- Remove customer from the current bank." << endl;
    cout << "'P' -- Print all customers in the current bank to the monitor." << endl;
    cout << "'W' -- Make a withdraw." << endl;
    cout << "'D' -- Make a deposit." << endl;
    cout << "'S' -- Save current bank information to a file." << endl;
    cout << "'T' -- Sort list." << endl;
    cout << "'V' -- Print a customer's details in a page format to the monitor." << endl;
    cout << "'X' -- Exit the program." << endl;

    cout << "Choose a letter from the menu: " << flush;
    cin >> menu;
    cout << endl;
}

void chooseMenu(Customer Info[], Bank &BankInfo)
{
    char menu[2];
    do {
        // show the list of menu
        showMenu(menu);
        system("cls");
        // if second index is not empty outputs an error message
        if (menu[1] != NULL) {
            cout << "Please try again." << endl;
        }
        else {
            switch (toupper(menu[0])) {
                case 'A':
                    addCustomer(Info, BankInfo);
                    break;
                case 'R':
                    removeCustomer(Info, BankInfo);
                    break;
                case 'P':
                    displayTableFormat(Info, BankInfo);
                    break;
                case 'W':
                    withdraw(Info, BankInfo);
                    break;
                case 'D':
                    deposit(Info, BankInfo);
                    break;
                case 'S':
                    saveBankInfoToFile(Info, BankInfo);
                    break;
                case 'T':
                    sortCustomers(Info);
                    break;
                case 'V':
                    viewCustomerDetails(Info);
                    break;
                case 'X':
                    saveUpdateInputFile(Info, BankInfo);
                    exit(0);
                default:
                    cout << "Please choose a letter from the menu." << endl;
            }
        }
    } while (menu[0] != 'X');
}
