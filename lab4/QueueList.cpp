#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
    Customer* current = head;

    while (current != nullptr) {
        Customer* nextCustomer = current->get_next(); // Get the next customer before deleting current
        delete current; // Delete the current customer
        current = nextCustomer; // Move to the next customer
    }

    head = nullptr; // Avoid dangling pointer
}


Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head

  // check empty or not
  if (head == nullptr) {
    head = customer;
    return;
  }

  // find the last customer
  Customer* temp = head;
  while (temp->get_next() != nullptr) {
    temp = temp->get_next();
  }
  temp->set_next(customer);
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue
  // and return a pointer to it

  // check empty or not
  if (head == nullptr) {
    return nullptr;
  }

  Customer* temp = head;
  head = temp->get_next();
  temp->set_next(nullptr);
  return temp;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int total = 0;
  Customer* temp = head;
  while (temp != nullptr) {
    total += temp->get_numOfItems();
    temp = temp->get_next();
  }
  return total;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
