//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"


using namespace std;

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // Pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // Maximum number of nodes as set in the command line
int maxResistors = 0;  // Maximum number of resistors as set in the command line
int resistorsCount = 0;  // Count the number of resistors

string errorArray[10] = {
    "invalid command",                                        // 0
    "invalid argument",                                       // 1
    "negative resistance",                                    // 2
    "node value is out of permitted range",                   // 3
    "resistor name cannot be keyword \"all\"",                // 4
    "both terminals of resistor connect to same node",        // 5
    "too few arguments",                                      // 6
    "resistor name not found",                                // 7
    "node value is out of permitted range 1 - maxNodeNumber"  // 8
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);
void printError(int error);
Resistor* findResistor(const string& name);
void deleteAllResistors();
void handleSolve(stringstream& ss);
int getNodeID(Node *node);

int main() {
  cout << fixed << setprecision(2);
  string line;
  stringstream ss;

  while (true) {
    cout << ">>> ";
    cout.flush();
    getline(cin, line);
    if (line.empty()) {
      break;  // Exit loop if input is empty
    }
    if (line[line.size() - 1] == 13) {
      line = line.substr(0, line.size() - 1);
    }
    /*for(int i=0;i<line.size();i++){
      cout<<(int)line[i]<<" ";
    }
    cout<<endl;
    */

    ss.clear();
    ss.str(line);

    string command;
    if (!getString(ss, command)) {
      continue;  // Skip to next iteration if command retrieval fails
    }

    if (command == "maxVal") {
      handleMaxVal(ss);
    } else if (command == "insertR") {
      handleInsertR(ss);
    } else if (command == "modifyR") {
      handleModifyR(ss);
    } else if (command == "printR") {
      handlePrintR(ss);
    } else if (command == "deleteR") {
      handleDeleteR(ss);
    } else if (command == "setV") {
      handleSetV(ss);
    } 
    else if (command == "solve") {
      handleSolve(ss);
    } 
    else {
      printError(0);  // Invalid command
    }
  }

  // Clean up resources before exiting
  deleteAllResistors();
  delete[] nodes;
  delete[] resistors;
  nodes = nullptr;
  resistors = nullptr;

  return 0;
}

// Implementation of helper functions
bool getInteger(stringstream& ss, int& x) {
  if (ss.eof()) {
    printError(6);
    return false;
  }
  ss >> x;
  if (ss.fail()) {
    printError(1);
    return false;
  }
  return true;
}

bool getString(stringstream& ss, string& s) {
  if (ss.eof()) {
    printError(6);
    return false;
  }
  ss >> s;
  if (ss.fail()) {
    printError(1);
    ss.clear();
    return false;
  }
  return true;
}

bool getDouble(stringstream& ss, double& s) {
  if (ss.eof()) {
    printError(6);
    return false;
  }
  ss >> s;
  if (ss.fail()) {
    printError(1);
    ss.clear();
    return false;
  }
  return true;
}

void handleMaxVal(stringstream& ss) {
  int max_NodeNumber, max_Resistors;
  if (!getInteger(ss, max_NodeNumber) || !getInteger(ss, max_Resistors)) {
    return;
  }

  // Clean up previous arrays if they were allocated
  delete[] nodes;
  delete[] resistors;

  maxNodeNumber = max_NodeNumber;
  maxResistors = max_Resistors;

  nodes = new Node[maxNodeNumber];
  resistors = new Resistor*[maxResistors];

  for (int i = 0; i < maxResistors; i++) {
    resistors[i] = nullptr;
  }
  resistorsCount = 0;

  cout << "New network: max node number is " << maxNodeNumber
       << "; max resistors is " << maxResistors << endl;
}

void handleInsertR(stringstream& ss) {
  if (resistorsCount >= maxResistors) {
    printError(8);  // Too many resistors
    return;
  }
  string name;
  double resistance;
  int endpoints[2];

  if (!getString(ss, name)) {
    return;
  }
  if (name == "all") {
    printError(4);  // Resistor name cannot be "all"
    return;
  }
  if (!getDouble(ss, resistance)) {
    return;
  }
  if (resistance < 0.0) {
    printError(2);  // Negative resistance
    return;
  }

  if (!getInteger(ss, endpoints[0])) {
    return;
  }
  if (!getInteger(ss, endpoints[1])) {
    return;
  }
  if (endpoints[0] == endpoints[1]) {
    printError(5);  // Both terminals of resistor connect to the same node
    return;
  }
  if (endpoints[0] < 1 || endpoints[0] > maxNodeNumber || endpoints[1] < 1 ||
      endpoints[1] > maxNodeNumber) {
    printError(3);  // Out of permitted range
    return;
  }

  // Insert the resistor and update the node connections
  Resistor* resistor = new Resistor(name, resistance, endpoints);
  resistors[resistorsCount] = resistor;
  nodes[endpoints[0] - 1].addResistor(resistorsCount);
  nodes[endpoints[1] - 1].addResistor(resistorsCount);
  resistorsCount++;

  cout << "Inserted: resistor " << name << " " << resistance << " Ohms "
       << endpoints[0] << " -> " << endpoints[1] << endl;
}

void handleModifyR(stringstream& ss) {
  string r_name;
  double newResistance;

  if (!getString(ss, r_name) || !getDouble(ss, newResistance)) {
    return;
  }

  Resistor* targetR = findResistor(r_name);
  if (targetR == nullptr) {
    return;
  }

  cout << "Modified: resistor " << r_name << " from "
       << targetR->getResistance() << " Ohms to " << newResistance << " Ohms"
       << endl;

  targetR->setResistance(newResistance);
}

void handlePrintR(stringstream& ss) {
  string r_name;
  if (!getString(ss, r_name)) {
    return;
  }

  if (r_name == "all") {
    for (int i = 0; i < resistorsCount; i++) {
      if (resistors[i] != nullptr) {
        resistors[i]->print();
      }
    }
  } else {
    Resistor* targetR = findResistor(r_name);
    if (targetR == nullptr) {
      return;
    }
    targetR->print();
  }
}

void handleDeleteR(stringstream& ss) {
  string r_name;
  if (!getString(ss, r_name)) {
    return;
  }

  if (r_name == "all") {
    deleteAllResistors();
    cout << "Deleted: all resistors" << endl;
  } else {
    Resistor* targetR = findResistor(r_name);
    if (targetR == nullptr) {
      return;
    }
    delete targetR;
    targetR = nullptr;
    cout << "Deleted: resistor " << r_name << endl;
  }
}

void handleSetV(stringstream& ss) {
  int id;
  double v;
  if (!getInteger(ss, id)) {
    return;
  }
  if (id < 1 || id > maxNodeNumber) {
    printError(8);
    return;
  }
  if (!getDouble(ss, v)) {
    return;
  }
  cout << "Set: node " << id << " to " << v << " Volts" << endl;
  nodes[id - 1].setVoltage(v);

  //debug
  //cout<<nodes[id-1].getVoltage()<<endl;
}

void printError(int error) {
  string message = errorArray[error];
  cout << "Error: " << message << endl;
}

Resistor* findResistor(const string& name) {
  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i] != nullptr && resistors[i]->getName() == name) {
      return resistors[i];
    }
  }
  printError(7);
  return nullptr;
}

void deleteAllResistors() {
  for (int i = 0; i < resistorsCount; i++) {
    delete resistors[i];
    resistors[i] = nullptr;
  }
  resistorsCount = 0;
}


//




