//
//  Rectangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class here

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

#include "Rectangle.h"
#include "Shape.h"

// Cretae a rectangle object
Rectangle::Rectangle(string n, float xcent, float ycent, float w, float h)
    : Shape(n, xcent, ycent) {
  width = w;
  height = h;
};

Rectangle::~Rectangle() {
  // nothing to do
}

// get wigth and height
float Rectangle::getWidth() const { return width; };

float Rectangle::getHeight() const { return height; };

// set x and y
void Rectangle::setWidth(float w) { width = w; };

void Rectangle::setHeight(float h) { height = h; };

// utility methods
void Rectangle::draw() const {
  cout << std::fixed;
  cout << std::setprecision(2);

  cout << "rectangle: " << name << " " << x_centre << " " << y_centre << " "
       << width << " " << height << " " << computeArea() << endl;
};

float Rectangle::computeArea() const { return (width * height); };

Shape* Rectangle::clone() const { return (new Rectangle(*this)); };