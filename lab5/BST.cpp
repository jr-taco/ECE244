//
//  BST.cpp
//  Lab 5 Search Through A Database
//
//  Created by Author Name, Date
#include "BST.h"

#include <iostream>

#include "Employee.h"

using namespace std;

namespace {
bool lessThan(Employee* a, Employee* b, string order) {
  if (order == "ID") {
    if (a->getID() < b->getID()) return true;
    return false;
  }
  if (order == "name") {
    if (a->getName() < b->getName()) return true;
    return false;
  }
  if (order == "age") {
    if (a->getAge() < b->getAge()) return true;
    return false;
  }
}

void insertH(Employee* newEmployee, Employee* current, string order) {
  if (current == NULL) {
    current = newEmployee;
    cout << "this shouldn't happen" << endl;
    return;
  }
  if (lessThan(current, newEmployee, order)) {
    // insert into right
    if (current->getRight() == NULL)
      current->setRight(newEmployee);
    else
      insertH(newEmployee, current->getRight(), order);
  } else {
    // insert to the left
    if (current->getLeft() == NULL)
      current->setLeft(newEmployee);
    else
      insertH(newEmployee, current->getLeft(), order);
  }
}

void printH(Employee *current) {
  if (current == NULL) return;
  printH(current->getLeft());
  current->print();
  printH(current->getRight());
}

Employee *searchIDH(Employee *current, int ID) {
  if (current == NULL) return NULL;
  if (current->getID() == ID) return current;
  if (ID < current->getID()) return searchIDH(current->getLeft(), ID);
  return searchIDH(current->getRight(), ID);
}

void searchAgeRangeH(Employee *current, double lowAge, double highAge) {
  if (current == NULL) return;
  if (current->getAge() >= lowAge) {
    searchAgeRangeH(current->getLeft(), lowAge, highAge);
  }
  if (current->getAge() >= lowAge && current->getAge() <= highAge) {
    current->print();
  }
  if (current->getAge() <= highAge) {
    searchAgeRangeH(current->getRight(), lowAge, highAge);
  }
}

void autocompleteH(Employee *current, string prefix) {
  if (current == NULL) return;
  string name=current->getName();
  bool flag=true;

  for(int i=0;i<prefix.length();i++){
    if(i>= name.length() || name[i]!=prefix[i]){
      flag=false;
      break;
    }
  }

  if(flag){
    current->print();
  }

  autocompleteH(current->getLeft(),prefix);
  autocompleteH(current->getRight(),prefix);
}

}  // namespace

BST::BST(string order_) {
  root = NULL;
  order = order_;
}

BST::~BST() {
  // delete all the nodes in the tree
  delete root;
}

void BST::insert(Employee *newEmployee) {
  if (root == NULL) {
    root = newEmployee;
    return;
  }
  insertH(newEmployee, root, order);
}

void BST::printInOrder() { printH(root); }

Employee *BST::searchID(int ID) { return searchIDH(root, ID); }

void BST::searchAgeRange(double lowAge, double highAge) {
  // _printTree(root, 0);
  searchAgeRangeH(root, lowAge, highAge);
}

void BST::autocomplete(string prefix) { autocompleteH(root, prefix); }