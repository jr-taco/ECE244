//
//  Ball.cpp
//  Lab 2 Pong Game
//
//  Created by Author Name, Date

#include<iostream>
#include<cmath>
#include<cstdlib>
#include "Screen.h"
#include "Ball.h"
#include "Globals.h"

using namespace std;

double Ball:: getX(){
    return x;
}

int Ball:: getID(){
    return id;
}

Ball:: Ball(double x, double y, double velocity_x, double velocity_y, int id){
    this->x=x;
    this->y=y;
    this->velocity_x=velocity_x;
    this->velocity_y=velocity_y;
    this->id=id;
    height=1;
    width=1;
}

Ball:: Ball(){
    x=30.0;
    y=30.0;
    velocity_x=1.7;
    velocity_y=0;
    id=0;
    height=1;
    width=1;
}

void Ball::update(){
    velocity_y=velocity_y-0.02*timeStep;
    x+=velocity_x*timeStep;
    y+=velocity_y*timeStep;
}


int Ball::overlap(Ball&b){
    double x1=x;
    double y1=y;
    double vert_overlap=std::min(y1+height,b.y+height)-std::max(y1,b.y);
    double horiz_overlap=std::min(x1+width,b.getX()+width)-std::max(x1,b.getX());
    if(vert_overlap <= 0 ){
        vert_overlap=0;
        return NO_OVERLAP;
    }
    if(horiz_overlap <= 0){
        horiz_overlap=0;
        return NO_OVERLAP;
    }
    else if(vert_overlap < horiz_overlap){
        return HORIZONTAL_OVERLAP;
    }else{
        return VERTICAL_OVERLAP;
    }
}

int Ball::overlap(Player&p){
    double x1=x;
    double y1=y;
    double y2=p.getY();
    double x2=p.getX();    
    double vert_overlap=std::min(y1+height, y2+p.getHeight())-std::max(y1,y2);
    double horiz_overlap=std::min(x1+width,x2+p.getWidth())-std::max(x1,x2);
    //cout<<y1<<" "<<height<<" " <<y2<<" " <<p.getHeight() <<" "<<vert_overlap<<endl;
    if(vert_overlap <= 0 ){
        vert_overlap=0;
        return NO_OVERLAP;
    }
    if(horiz_overlap <= 0){
        horiz_overlap=0;
        return NO_OVERLAP;
    }
    if(vert_overlap < horiz_overlap){
        return HORIZONTAL_OVERLAP;
    }else{
        return VERTICAL_OVERLAP;
     }
}

void Ball::bounce(Ball arr[], int ballCount, Player player){
    if(x<=0 || x+width>=WIDTH){ //x
        velocity_x=-velocity_x;
    }
    if(y<=0 || y+height>=HEIGHT){ //y
        velocity_y=-velocity_y;
    }
    if(overlap(player)!=NO_OVERLAP){
        velocity_x=-velocity_x;
    }
    for (int i = 0; i < ballCount; i++) {
        if (i == id) {
            continue;
        }
        int overlap_mode = arr[i].overlap(*this);
        if (overlap_mode == HORIZONTAL_OVERLAP) {
            velocity_x = -velocity_x;
        } else if (overlap_mode == VERTICAL_OVERLAP) {
            velocity_y = -velocity_y;
        }
    }
}


void Ball::draw(Screen& screen_to_draw_to) {
    screen_to_draw_to.addPixel(x, y, 'o');
}