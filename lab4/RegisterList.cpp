#include "RegisterList.h"
#include <limits.h>
#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  Register* current = head;
  while (current != nullptr) {
    Register* nextRegister = current->get_next(); // Get the next register before deleting current
    delete current; // Delete the current register
    current = nextRegister; // Move to the next register
  }
  head = nullptr; // Avoid dangling pointer
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() {
  // return number of registers
  return size;
}

Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items

  // check whether the list is empty
  if (head == nullptr) {
    return nullptr;
  }

  Register* result = head;
  int least = head->get_queue_list()->get_items();
  Register* temp = head->get_next();
  while (temp != nullptr) {
    int current = temp->get_queue_list()->get_items();
    if (current < least) {
      least = current;
      result = temp;
    }
    temp = temp->get_next();
  }
  return result;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* temp = head;
  while (temp != nullptr) {
    if (temp->get_queue_list()->get_items() == 0) {
      return temp;
    }
    temp = temp->get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size
  if (head == NULL) {
    head = newRegister;
  } else {
    Register* temp = head;
    while (temp->get_next() != nullptr) {
      temp = temp->get_next();
    }
    temp->set_next(newRegister);
  }
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  Register* temp = head;
  while (temp != nullptr) {
    if (temp->get_ID() == ID) {
      return true;
    }
    temp = temp->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID
  // return the dequeued register
  // return nullptr if register was not found

  // check the list is empty or not
  if (head == nullptr) {
    return nullptr;
  }

  if(!foundRegister(ID)){
    return nullptr;
  }

  // check if head is needed to remove
  if (head->get_ID() == ID) {
    Register* temp = head;
    head = head->get_next();
    size--;
    temp->set_next(nullptr);
    return temp;
  }

  // check other parts of the list
  Register* temp = head;
  while (temp->get_next() != nullptr && temp->get_next()->get_ID() != ID) {
    temp = temp->get_next();
  }

  if (temp->get_next() == nullptr) {
    return nullptr;
  }

  Register* result = temp->get_next();
  temp->set_next(temp->get_next()->get_next());
  size--;
  result->set_next(nullptr);  // detach

  return result;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  //  check the list is empty or not
/* if (head == nullptr) {
    return nullptr;
  }

  bool free = true;
  Register* temp = head;
  while (temp != nullptr) {
    if (temp->calculateDepartTime() != -1) {
      free = false;
      break;
    }
    temp = temp->get_next();
  }

  if (free) {
    return nullptr;
  }

  Register* result = head;
  double minTime = INT_MAX;
  while (temp != nullptr) {
    double current = temp->calculateDepartTime();
    if (current == -1) {
      temp = temp -> get_next();
      continue;
    } if (current < minTime) {
      minTime = current;  
      result = temp;
    }
    temp = temp -> get_next();
  }
  return result;
  */
 
  // Check if the list is empty
  if (head == nullptr) {
    return nullptr;
  }

  // Check if all registers are free
  Register* temp = head;
  bool allFree = true;
  while (temp != nullptr) {
    if (temp->calculateDepartTime() != -1) {
      allFree = false;
      break;
    }
    temp = temp->get_next();
  }

  if (allFree) {
    return nullptr;
  }

  // Find the register with the minimum departure time
  Register* result = nullptr;
  double minTime = INT_MAX;

  temp = head;  // Reset temp to start at the head of the list again
  while (temp != nullptr) {
    double currentDepartTime = temp->calculateDepartTime();
    if (currentDepartTime != -1 && currentDepartTime < minTime) {
      minTime = currentDepartTime;
      result = temp;
    }
    temp = temp->get_next();
  }

  return result;
}
 


void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
