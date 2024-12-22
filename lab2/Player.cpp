//
//  Player.cpp
//  Lab 2 Pong Game
//
//  Created by Author Name, Date

#include "Player.h"
#include "Screen.h"
#include "Globals.h"
#include <string>
#include <iostream>

using namespace std;

double Player:: getX(){
    return x;
}

double Player:: getY(){
    return y;
}

int Player:: getHeight(){
    return height;
}

int Player:: getWidth(){
    return width;
}

Player:: Player(double x, double y, int height){
    this->x=x;
    this->y=y;
    this->height=height;
    width=1;
}

void Player:: decreaseHeight(int delta_to_decrease_by){
    height=height-delta_to_decrease_by;
    if(height <3){
        height=3;
    }
}

void Player:: update(char c) {
    if(c=='A'){
        y=y+2;
        if(y>=HEIGHT-height){
            y=HEIGHT-height;
        }
    }else if(c=='B'){
        y=y-2;
        if(y<=0){
            y=0;
        }
    }
}

void Player:: draw(Screen& screen_to_draw_to) {
    for(int i=0; i<height; i++){
        screen_to_draw_to.addPixel(x, y+i, '#');
    }
    
}