
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() {
  delete queue;
}

QueueList* Register::get_queue_list() { return queue; }

Register* Register::get_next() {     // Assuming this is in the Register class
    if (this == nullptr) {
        return nullptr; // Don't try to get next if this is null
    }
    return next;}

int Register::get_ID() { return ID; }

double Register::get_secPerItem() { return secPerItem; }

double Register::get_overheadPerCustomer() { return overheadPerCustomer; }

double Register::get_availableTime() { return availableTime; }

void Register::set_next(Register* nextRegister) { next = nextRegister; }

void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue
  if (queue == nullptr || queue->get_head() == nullptr) {
    return -1;
  }

  Customer* firstCustomer = queue->get_head();
  int items = firstCustomer->get_numOfItems();
  int arrival = firstCustomer->get_arrivalTime();
  double departure;
  if (arrival > availableTime) {
    departure = arrival + items * secPerItem + overheadPerCustomer;
  } else {
    departure = availableTime + items * secPerItem + overheadPerCustomer;
  }

  return departure;
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register
  if (queue == nullptr || queue->get_head() == nullptr) {
    return;
  }

  double departmentTime = calculateDepartTime();
  Customer* customer = queue->dequeue();
  customer->set_departureTime(departmentTime);
  doneList->enqueue(customer);
  availableTime = departmentTime;
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
