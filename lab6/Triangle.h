//
//  Triangle.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#ifndef Triangle_h
#define Triangle_h

#include <iostream>
#include <string>
using namespace std;

// This class implements the triangle Shape objects.
// ECE244 Student: Write the definition of the class here.

#include "Shape.h"

class Triangle : public Shape {
 private:
  float ax, ay, bx, by, cx, cy;

 public:
  Triangle(string n, float x1, float x2, float x3, float y1, float y2,
           float y3);

  virtual ~Triangle();
  void setVertices(float x1, float x2, float x3, float y1, float y2, float y3);

  // Mutator
  virtual void draw() const;
  virtual float computeArea() const;
  virtual Shape* clone() const;
};

#endif /* Triangle_h */
