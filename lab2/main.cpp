//
//  main.cpp
//  Lab 2 Pong Game
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-11
//  Copyright © 2024 Nathan Hung. No rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <string>
#include "Ball.h"
#include "Globals.h"
#include "Player.h"

#include "Screen.h"
#include "io.h"
#include <chrono>
#include <thread>

#include <iostream>

using namespace std;

bool check_hit_left_wall(Ball balls[], int ballCount){
  for(int i=0;i<ballCount;i++){
    if(balls[i].getX()<=0){
      return true;
    }
  }
  return false;
}

int main() {
  int score = 0;
  bool gameEnded = false;
  std::string messageToPutOnScreen;

  std::chrono::steady_clock::time_point begin_time =
      std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point now_time;
  Screen screen = Screen();

  int ballCount = 0;
  const int max_ballCount = 5;

  Ball balls_array[max_ballCount];

  balls_array[0] = Ball();
  ballCount++;

  Player player = Player(0, 5, 10);

  while (!gameEnded) {
    // Loop through simulation_fps iterations within a frame
    for (int frame = 0; frame < simulation_fps; frame++) {
      char input = get_input();  // Handle user input

      // If 'q' is pressed, quit the game
      if (input == 'q') {
        gameEnded = true;
        continue;
      }

      // If valid input, update the player paddle
      if (input != '\0') {
        player.update(input);
      }

      // Update ball positions and check for collisions
      for (int i = 0; i < ballCount; i++) {

        // Check if the ball overlaps with the player's paddle
        if (balls_array[i].overlap(player) != NO_OVERLAP) {
          score++; 

          // Every 2 hits, decrease the paddle's height
          if (score % 2 == 0 && player.getHeight() > 3) {
            player.decreaseHeight(1);
          }

          // Every 5 hits, add a new ball
          if (score % 5 == 0 && ballCount < max_ballCount) {
            balls_array[ballCount] = Ball(30.0, 30.0, 0.9, 0.0, ballCount);
            ballCount++;
          }
        }
      }

      for (int i = 0; i < ballCount; i++) {
        balls_array[i].bounce(balls_array, ballCount, player);
        balls_array[i].update();
      }
      

      // ball hits the left wall
      for (int i = 0; i < ballCount; i++) {
        if (balls_array[i].getX() <= 0) {
          gameEnded = true;  // End the game if a ball hits the left wall
          break;
        }
      }
    }

    // Draw the balls and player on the screen
    for (int i = 0; i < ballCount; i++) {
      balls_array[i].draw(screen);
    }
    player.draw(screen);      

    messageToPutOnScreen = "Your score is: " + std::to_string(score);
    screen.update(messageToPutOnScreen);
    //cout<<balls_array[0].overlap(player)<<endl;
    //cout<<"x:"<<balls_array[0].getX()<<endl;
    

    // Wait until one frame time has passed
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - begin_time)
               .count() < (long int)milliseconds_per_frame) {
    }

    // Reset the frame timer
    begin_time = std::chrono::steady_clock::now();
  }
  messageToPutOnScreen = "Game over. Your score is: " + std::to_string(score);
  screen.update(messageToPutOnScreen);

  return 0;
}