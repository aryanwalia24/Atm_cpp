#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Account
{
    string pin;
    double balance;
    vector<string> transaction;
};

class TransactionHistory
{
public:
    static void showTransactionHistory(const string &accN, map<string, Account> &database)
    {
        cout << "Transaction History for Account Number " << accN << " :\n"
             << endl;
        for (const string &transaction : database[accN].transaction)
        {
            cout << transaction << endl;
        }
    }
};

class Withdrawal
{
public:
    static void withdraw(const string &accN, double amount, map<string, Account> &database)
    {
        if (amount <= 0)
        {
            cout << "\nInvalid amount. Withdrawal failed." << endl;
            return;
        }
        if (amount > database[accN].balance)
        {
            cout << "\nInsufficient balance. Withdrawal failed." << endl;
            return;
        }
        database[accN].balance -= amount;
        
        string transaction = "Withdrawal of $" + to_string(amount);
        database[accN].transaction.push_back(transaction);
        
        cout << "\nWithdrawal successful. New balance: \n$" << database[accN].balance << endl;
    }
};

class Deposit
{
public:
    static void deposit(const string &accN, double amount, map<string, Account> &database)
    {
        if (amount <= 0)
        {
            cout << "\nInvalid amount. Deposit failed." << endl;
            return;
        }
        database[accN].balance += amount;
        
        string transaction = "Deposit of $" + to_string(amount);
        database[accN].transaction.push_back(transaction);
        
        cout << "Deposit successful. New balance: \n$" << database[accN].balance << endl;
    }
};

class Transfer
{
public:
    static void transfer(const string &accN1, const string &accN2, double amount, map<string, Account> &database)
    {
        if (amount <= 0)
        {
            cout << "\nInvalid amount. Transfer failed." << endl;
            return;
        }
        if (accN1 == accN2)
        {
            cout << "\nCannot transfer to the same account. Transfer failed." << endl;
            return;
        }
        if (!(database.count(accN2) > 0))
        {
            cout << "\nInvalid recipient account number. Transfer failed." << endl;
            return;
        }
        if (amount > database[accN1].balance)
        {
            cout << "\nInsufficient balance. Transfer failed." << endl;
            return;
        }
        database[accN1].balance -= amount;
        database[accN2].balance += amount;
        
        string transaction1 = "Transfer of $" + to_string(amount) + " to Account " + accN2;
        string transaction2 = "Received transfer of $" + to_string(amount) + " from Account " + accN1;
        
        database[accN1].transaction.push_back(transaction1);
        database[accN2].transaction.push_back(transaction2);
        
        cout << "\nTransfer successful. New balance for Account " << accN1 << ": $" << database[accN1].balance << endl;
        cout << "New balance for Account " << accN2 << ": $\n"
             << database[accN2].balance << endl;
    }
};

class ATM
{
private:
    map<string, Account> database;

public:
    ATM()
    {
        database["1001"] = {"123456", 0.0, {}};
        database["1002"] = {"111111", 1000.0, {}};
        database["1003"] = {"222222", 2000.0, {}};
        database["1004"] = {"333333", 3000.0, {}};
        database["1005"] = {"444444", 4444.0, {}};
        database["1006"] = {"555555", 6789.0, {}};
        database["1007"] = {"666666", 10000.0, {}};
        database["1008"] = {"777777", 12140.0, {}};
        database["1009"] = {"888888", 350.0, {}};
        database["1010"] = {"987650", 5000.0, {}};
    }
    void addUser(const string &UserID, const string &pin, const double &balance)
    {
        Account newUser;
        newUser.pin = pin;
        newUser.balance = balance;
        database.insert(make_pair(UserID, newUser));
    }

    bool userAuthenticator(const string &userId, const string &pin)
    {
        if (database.count(userId) > 0)
        {
            if (database[userId].pin == pin)
            {
                return true;
            }
        }
        return false;
    }

    bool validPin(const string &pin)
    {
        int len = pin.length();
        for (char c : pin)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return len == 6;
    }

    bool validAccn(const string &accN, map<string, Account> &database)
    {
        return database.count(accN) > 0;
    }

    bool exitf()
    {
        string choice;
        cout << "\nDo you want to Exit (Y or N) : ";
        cin >> choice;
        return (choice == "y" || choice == "Y");
    }

    void run()
    {
        while (true)
        {
            string accN;
            cout << "\nEnter Your Account Number: ";
            cin >> accN;
            if (accN == "exit" || accN == "EXIT" || accN == "quit" || accN == "QUIT")
            {
                break;
            }
            if (!validAccn(accN, database))
            {
                cout << "\nAccount Number not found in the database. Please try again." << endl;
                continue;
            }
            string pin;
            cout << "Enter your 6-digit PIN: ";
            cin >> pin;
            if (pin == "exit" || pin == "EXIT" || pin == "quit" || pin == "QUIT")
            {
                break;
            }
            if (!validPin(pin))
            {
                cout << "\nInvalid PIN. Please try again." << endl;
                continue;
            }
            if (userAuthenticator(accN, pin))
            {
                cout << "\n--------> Access granted <--------\n"
                     << endl;
                cout << "Choice of Actions :" << endl;
                cout << "1. TRANSACTIONS HISTORY" << endl;
                cout << "2. WITHDRAW" << endl;
                cout << "3. DEPOSIT" << endl;
                cout << "4. TRANSFER" << endl;
                cout << "5. QUIT\n"
                     << endl;

                cout << "Enter no. acc to your choice of Action : ";
                string action;
                cin >> action;
                int actionNum = stoi(action);

                if (actionNum == 4)
                {
                    string recipientAccN;
                    double transferAmount;
                    
                    cout << "\nEnter the recipient's account number: ";
                    cin >> recipientAccN;
                    
                    cout << "Enter the amount to transfer: ";
                    cin >> transferAmount;
                    Transfer::transfer(accN, recipientAccN, transferAmount, database);
                    if (exitf())
                        return;
                    else
                        continue;
                }

                switch (actionNum)
                {
                case 1:
                    TransactionHistory::showTransactionHistory(accN, database);
                    if (exitf())
                        return;
                    else
                        continue;
                case 2:
                    double withdrawalAmount;
                    cout << "\nEnter the amount to withdraw: ";
                    cin >> withdrawalAmount;
                    Withdrawal::withdraw(accN, withdrawalAmount, database);
                    if (exitf())
                        return;
                    else
                        continue;
                case 3:
                    double depositAmount;
                    cout << "\nEnter the amount to deposit: ";
                    cin >> depositAmount;
                    Deposit::deposit(accN, depositAmount, database);
                    if (exitf())
                        return;
                    else
                        continue;
                case 5:
                    break;
                default:
                    cout << "\nInvalid action. Please try again." << endl;
                    exitf();
                }
            }
            else
            {
                cout << "\nWrong PIN. Please try again." << endl;
                continue;
            }
            break;
        }
    }
};

void greetings()
{
    string line(41, '-'); 

    cout << "\n"
         << line << endl;
    cout << "|                                       |" << endl;
    cout << "|          Welcome to the ATM!          |" << endl;
    cout << "|                                       |" << endl;
    cout << line << endl;
}

int main()
{
    ATM atm;
    greetings();
    atm.run();
//Bank Functiionality 
    // string s1 = "#";
    
    // cout << "\nAlready a User: Y or N " << endl;
    // cout << "ANS : ";
    // cin >> s1;

    // if (s1 == "y" || s1 == "Y")
    // {
    //     atm.run();
    // }
    // else if (s1 == "n" || s1 == "N")
    // {
    //     string accN = "#";
    //     string pin = "#";
    //     double amount = 0;
        
    //     cout << "\nFor a New User \n";
    //     cout << "Enter Account Number/User ID : ";
    //     cin >> accN;
        
    //     cout << "Enter 6 digit pin : ";
    //     cin >> pin;
        
    //     cout << "Enter starting Balance : ";
    //     cin >> amount;
        
    //     atm.addUser(accN, pin, amount);
    //     atm.run();
    // }
    // else
    // {
    //     cout << "Invalid Choice!" << endl;
    // }
    return 0;
}
