//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: 
// Date:

#include "Node.h" 
#include <iostream>

using namespace std;

Node:: Node(){
    numRes=0;
    voltage=0;
    voltageIsSet=false; 
}  // sets numRes to 0, voltage to 0 and voltageIsSet to false
bool Node:: canAddResistor(){
    if(numRes<MAX_RESISTORS_PER_NODE){
        return true;
    }
    else{
        return false;
    }
}  // returns if the number of resistors is less than MAX_RESISTORS_PER_NODE
void Node::addResistor(int rIndex){
    resIDArray[numRes]=rIndex;
    numRes++;                     // adds a resistor to resIDArray array and increments number of resistors
}                                 
double Node::getVoltage(){
    return voltage;
}  // returns voltage

void Node::setVoltage(double voltage_){
    this->voltage=voltage_;
    voltageIsSet=true;
}  // sets voltage and sets voltageIsSet to true

void Node::setVoltageInternally(double voltage_){ // sets voltage temporarily during iterations for solve
    this->voltage=voltage;
}
                         // command
bool Node::isVoltageSet(){
    return voltageIsSet;
}  // returns voltageIsSet

int Node::getNumRes(){
    return numRes;
}       // returns numRes

int* Node::getResIDArray(){
    return resIDArray;
}  // returns resIDArray