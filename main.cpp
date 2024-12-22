#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <limits.h>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// debug
//#define ECHO_FILE
//#define TIMESTAMP

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// data count
void printStatistics();

// time update
void time_update(double expectedTime, string mode);

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation
                        // //全局时间

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

// debug
#ifdef ECHO_FILE
  cout << line << endl;
#endif

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);

// debug
#ifdef ECHO_FILE
    cout << line << endl;
#endif
  }

  // Print statistics
  printStatistics();

  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  delete registerList;
  delete doneList;
  delete singleQueue;

  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

// debug
#ifdef ECHO_FILE
  cout << mode << endl;
#endif

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  Customer* customer = new Customer(expTimeElapsed + timeElapsed, items);
  time_update(customer -> get_arrivalTime(), mode);
  cout << "A customer entered" << endl;

  if (mode == "single") {
    singleQueue -> enqueue(customer);
    Register* temp = registerList -> get_free_register();
    if (temp == nullptr) {
      cout << "No free registers" << endl;
    } else {
      customer = singleQueue -> dequeue();
      temp -> get_queue_list() -> enqueue(customer);
      temp -> set_availableTime(expTimeElapsed);
      cout << "Queued a customer with free register " << temp -> get_ID() << endl;
    }
  } else {
    //要去找最快的那个register
    Register* fast = registerList -> get_head();
    int leastItems = INT_MAX;
    Register* current = fast;
      // Traverse through all registers to find the one with the least number of items
    while (current != nullptr) {
      int numItems = current->get_queue_list()->get_items();  // Use get_items() to get the number of items in the queue

      if (numItems < leastItems) {
        leastItems = numItems;
        fast = current;
      }

      current = current->get_next();  // Move to the next register
    }

    // Queue the customer to the register with the least number of items
    fast->get_queue_list()->enqueue(customer);
    cout << "Queued a customer with quickest register " << fast->get_ID() << endl;
  }
  #ifdef TIMESTAMP
    cout << "expTime: " << expTimeElapsed << endl;
  #endif
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  if (registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }

  // 更新时间  开新的register
  Register *newRegister = new Register(ID, secPerItem, setupTime, expTimeElapsed+timeElapsed);
  time_update(newRegister->get_availableTime(), mode);
  cout << "Opened register " << ID << endl;
  registerList->enqueue(newRegister);
  

  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register
  if (mode == "single") {
    if (singleQueue->get_head() != nullptr) {
      cout << "Queued a customer with free register " << ID << endl;
      newRegister->get_queue_list()->enqueue(singleQueue->dequeue());
    }
  }

  //delete newRegister;
  #ifdef TIMESTAMP
    cout << "expTime: " << expTimeElapsed << endl;
  #endif
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message
  if (!registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is not open" << endl;
  } else {
    // 更新时间
    time_update(expTimeElapsed + timeElapsed, mode);
    Register *newRegister = registerList->dequeue(ID);
    delete newRegister;
    cout << "Closed register " << ID << endl;
  }
  #ifdef TIMESTAMP
    cout << "expTime: " << expTimeElapsed << endl;
  #endif
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void time_update(double expectedTime, string mode) {
  // simulate until the expected time
  // note that the only thing could happen is depart customers and enter customers
  while (expTimeElapsed < expectedTime) {
    // update:
    if (mode == "single") {
      while (singleQueue -> get_head() !=nullptr && registerList -> get_free_register() != nullptr) {
        Register* temp = registerList -> get_free_register();
        Customer* customer = singleQueue -> dequeue();
        temp -> get_queue_list() -> enqueue(customer);
        temp -> set_availableTime(expTimeElapsed);
        cout << "Queued a customer with free register " << temp -> get_ID() << endl;
        #ifdef TIMESTAMP
          cout << "expTime: " << expTimeElapsed << endl;
        #endif
      }
    }
    // get the next update time
    Register* temp = registerList -> calculateMinDepartTimeRegister(expTimeElapsed);
    if (temp == nullptr) {
      // means that no more action needed
      expTimeElapsed = expectedTime;
      continue;
    }
    if (temp -> calculateDepartTime() > expectedTime) {
      expTimeElapsed = expectedTime;
      continue;
    }
    expTimeElapsed = temp -> calculateDepartTime();
    temp -> departCustomer(doneList);
    cout << "Departed a customer at register ID " << temp -> get_ID() << " at " << expTimeElapsed << endl;
    int a = 0;
    #ifdef TIMESTAMP
      cout << "expTime: " << expTimeElapsed << endl;
    #endif
  }
  if (mode == "single") {
    while (singleQueue -> get_head() !=nullptr && registerList -> get_free_register() != nullptr) {
      Register* temp = registerList -> get_free_register();
      Customer* customer = singleQueue -> dequeue();
      temp -> get_queue_list() -> enqueue(customer);
      temp -> set_availableTime(expTimeElapsed);
      cout << "Queued a customer with free register " << temp -> get_ID() << endl;
      #ifdef TIMESTAMP
        cout << "expTime: " << expTimeElapsed << endl;
      #endif
    }
  }
}

void printStatistics() {
  int maxWait = 0;
  double totalWait = 0.0;
  double sumSquares = 0.0;
  int count = 0;

  Customer *current = doneList->get_head();
  while (current != nullptr) {
    double waitTime = current->get_departureTime() - current->get_arrivalTime();
    if (waitTime > maxWait) {
      maxWait = waitTime;
    }
    totalWait += waitTime;
    sumSquares += waitTime * waitTime;
    count++;

    current = current->get_next();
  }
  double avgWait = totalWait / count;
  double variance = (sumSquares / count) - (avgWait * avgWait);
  double stddev = sqrt(variance);
  
  cout<<"Finished at time "<<expTimeElapsed<<endl;
  cout << "Statistics:" << endl;
  cout << "Maximum wait time: " << maxWait << endl;
  cout << "Average wait time: " << avgWait << endl;
  cout << "Standard Deviation of wait time: " << stddev << endl;
}

