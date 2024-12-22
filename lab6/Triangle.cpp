//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

#include "Shape.h"
#include "Triangle.h"

Triangle::Triangle(string n, float x1, float x2, float x3, float y1, float y2,
                   float y3)
    : Shape(n, (x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3) {
  ax = x1;
  ay = y1;
  bx = x2;
  by = y2;
  cx = x3;
  cy = y3;
}

Triangle::~Triangle() {
  // nothing to do
}

void Triangle ::setVertices(float x1, float x2, float x3, float y1, float y2,
                            float y3) {
  ax = x1;
  ay = y1;
  bx = x2;
  by = y2;
  cx = x3;
  cy = y3;
  x_centre = (x1 + x2 + x3) / 3.0;
  y_centre = (y1 + y2 + y3) / 3.0;
}

void Triangle ::draw() const {
  cout << std::fixed;
  cout << std::setprecision(2);

  cout << "triangle: " << name << " " << x_centre << " " << y_centre << " "
       << ax << " " << ay << " " << bx << " " << by << " " << cx << " " << cy
       << " " << computeArea() << endl;
}

float Triangle::computeArea() const {
  return 0.5 * abs(ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
}

Shape* Triangle::clone() const { return (new Triangle(*this)); }