#include <iostream>
using namespace std;

// Node class for singly linked list of accounts
class Node {
private:
    int AccNo;      // Account number
    int balance;    // Account balance
    Node* next;     // Pointer to next node in the list

public:
    // Constructor to initialize Node
    Node(int acc) {
        AccNo = acc;
        balance = 1000;  // Initial balance set to 1000
        next = nullptr;
    }

    friend class SLL;   // Allow SLL to access private members
};

// Singly Linked List class for managing accounts
class SLL {
private:
    Node* first;    // Pointer to the first node
    Node* last;     // Pointer to the last node
    int len;        // Length of the linked list

public:
    // Constructor to initialize SLL
    SLL() {
        first = last = nullptr;
        len = 0;
    }

    // Method to add a new account node
    void pushNode(int acc) {
        Node* newNode = new Node(acc);
        if (len == 0) {
            first = last = newNode;
        } else {
            last->next = newNode;
            last = newNode;
        }
        len++;
    }

    // Method to find a node by account number
    Node* findNode(int acc) {
        Node* temp = first;
        while (temp != nullptr) {
            if (temp->AccNo == acc) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // Method to perform deposit or withdrawal transaction
    void transaction(int acc, char m, int val) {
        Node* processNode = findNode(acc);
        if (processNode != nullptr) {
            if (m == 'D') {
                processNode->balance += val;    // Deposit
            } else if (m == 'W') {
                processNode->balance -= val;    // Withdrawal
            }
        }
    }

    // Method to print number of accounts with balance >= x
    void PrintBalMoreX(int x) {
        int count = 0;
        Node* temp = first;
        while (temp != nullptr) {
            if (temp->balance >= x) {
                count++;
            }
            temp = temp->next;
        }
        cout << count << endl;
    }

    // Method to print account number(s) with maximum balance
    void PrintMaxBalance() {
        int maxBalance = -1;
        Node* temp = first;

        // Find maximum balance
        while (temp != nullptr) {
            if (temp->balance > maxBalance) {
                maxBalance = temp->balance;
            }
            temp = temp->next;
        }

        // Print account number(s) with maximum balance
        temp = first;
        while (temp != nullptr) {
            if (temp->balance == maxBalance) {
                cout << temp->AccNo << " ";
            }
            temp = temp->next;
        }
        cout << endl;
    }

    // Method to print balance of account with given account number
    void PrintBalX(int x) {
        Node* temp = findNode(x);
        if (temp != nullptr) {
            cout << temp->balance << endl;
        }
    }
};

// Node class for doubly linked list of transactions
class Dnode {
private:
    int Acc;        // Account number
    char Action;    // Transaction type (Deposit 'D' or Withdrawal 'W')
    int Amount;     // Amount of transaction
    Dnode* next;    // Pointer to next node
    Dnode* prev;    // Pointer to previous node

public:
    // Constructor to initialize Dnode
    Dnode(int acc = -1, char act = 'A', int val = -1) {
        Acc = acc;
        Action = act;
        Amount = val;
        next = prev = nullptr;
    }

    friend class DLL;   // Allow DLL to access private members
};

// Doubly Linked List class for managing transactions
class DLL {
private:
    Dnode* head;        // Pointer to head sentinel node
    Dnode* tail;        // Pointer to tail sentinel node
    Dnode* cursor;      // Pointer to current node
    int dlen;           // Length of the doubly linked list
    int cursoridx;      // Index of current node

public:
    // Constructor to initialize DLL
    DLL() {
        head = new Dnode();     // Head sentinel node
        tail = new Dnode();     // Tail sentinel node
        head->next = tail;
        tail->prev = head;
        cursor = head;
        dlen = 0;
        cursoridx = 0;
    }

    // Method to add a new transaction node
    void pushDNode(int a, char b, int c) {
        Dnode* newNode = new Dnode(a, b, c);
        newNode->prev = tail->prev;
        tail->prev->next = newNode;
        newNode->next = tail;
        tail->prev = newNode;
        dlen++;
    }

    // Method to process next x transactions on account list l1
    void ProcessX(int x, SLL& l1) {
        while (cursor->next != tail && x > 0) {
            cursor = cursor->next;
            cursoridx++;
            l1.transaction(cursor->Acc, cursor->Action, cursor->Amount);
            x--;
        }
    }

    // Method to undo last y transactions on account list l1
    void UndoY(int y, SLL& l1) {
        while (cursor != head && y > 0) {
            l1.transaction(cursor->Acc, cursor->Action, -(cursor->Amount));
            cursor = cursor->prev;
            cursoridx--;
            y--;
        }
    }

    // Method to insert a new transaction node at position k
    void InsertNodeK(int a, char b, int c, int k, SLL& l1) {
        if (k >= 1 && k <= dlen) {
            int i;
            Dnode* addNode = new Dnode(a, b, c);
            Dnode* temp = head;
            for (i = 0; i < k; i++) {
                temp = temp->next;
            }
            temp->next->prev = addNode;
            addNode->next = temp->next;
            temp->next = addNode;
            addNode->prev = temp;
            if (temp != cursor) {
                l1.transaction(a, b, c);
            }
            dlen++;
        }
    }

    // Method to delete m transactions for account acc on account list l1
    void DeleteAM(int acc, int m, SLL& l1) {
        Dnode* temp = cursor->next;
        while (temp != tail && m != 0) {
            if (temp->Acc == acc) {
                Dnode* DelNode = temp;
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp = temp->next;
                delete DelNode;
                dlen--;
                m--;
            } else {
                temp = temp->next;
            }
        }
        cursor->next = temp;
    }

    // Method to process all transactions on account list l1
    void ProcessAll(SLL& l1) {
        ProcessX(dlen, l1);
    }

    // Method to print all transactions for account acc
    void PrintAllofY(int acc) {
        Dnode* temp = head->next;
        while (temp != tail) {
            if (temp->Acc == acc) {
                cout << temp->Acc << " " << temp->Action << " " << temp->Amount << endl;
            }
            temp = temp->next;
        }
    }

};

int main() {
    int c, n, acs, am, x, y, m, k;
    char choice, dw;
    SLL Accounts;    // Create an instance of SLL for accounts
    DLL Transactions;   // Create an instance of DLL for transactions

    // Input number of accounts and initialize accounts list
    cin >> c;
    while (c--) {
        cin >> acs;
        Accounts.pushNode(acs);
    }

    // Input number of transactions and initialize transactions list
    cin >> n;
    while (n--) {
        cin >> acs >> dw >> am;
        Transactions.pushDNode(acs, dw, am);
    }

    // Process user commands until 'E' (exit) is encountered
    while (true) {
        cin >> choice;
        switch (choice) {
            case 'F':   // Process next x transactions
                cin >> x;
                Transactions.ProcessX(x, Accounts);
                break;

            case 'R':   // Undo last y transactions
                cin >> y;
                Transactions.UndoY(y, Accounts);
                break;

            case 'I':   // Insert transaction at position k
                cin >> acs >> dw >> am >> k;
                Transactions.InsertNodeK(acs, dw, am, k, Accounts);
                break;

            case 'D':   // Delete m transactions for account acs
                cin >> acs >> m;
                Transactions.DeleteAM(acs, m, Accounts);
                break;

            case 'C':   // Process all remaining transactions
                Transactions.ProcessAll(Accounts);
                break;

            case 'S':   // Print all transactions for account y
                cin >> y;
                Transactions.PrintAllofY(y);
                break;

            case 'G':   // Print number of accounts with balance >= x
                cin >> x;
                Accounts.PrintBalMoreX(x);
                break;

            case 'M':   // Print account number(s) with maximum balance
                Accounts.PrintMaxBalance();
                break;

            case 'V':   // Print balance of account with account number x
                cin >> x;
                Accounts.PrintBalX(x);
                break;

            case 'E':   // Exit the program
                return 0;
        }
    }

    return 0;
}
