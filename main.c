/**
*   File: main.c (may change name)
*   Author: Patrick Smith
*   Last Updated: 4/30/2025
*   Purpose: Simple SDL2 Pong Game
*
*   Description:
*   This program makes a simple Pong game
*   Add more info later
*
*   Dependencies:
*   - SDL2 library
*
*   Compile With:
*   Find this out
**/

#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<stdio.h>
#include<stdbool.h>

void move(int amount);
void checkBounds();
void updateBall();


const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 500;


//creates Ball Structure
    typedef struct {
        int x, y;
        int radius;
        int xSpeed, ySpeed;
    } Ball;
Ball ball = {400, 300, 20, 5, 5};
SDL_Renderer* renderer = NULL;

int main(){
    //Initialized SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ball Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    move(1000);


    SDL_Delay(100);
    //Should Hack it to stay up May need to comment
    //SDL_Event e; bool quit = false; while(quit == false){
     //       while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT) quit = true;}}


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
/** \brief Moves the ball a specified amount of times by adding the speed
 *  (xSpeed / ySpeed) to the balls x or y position
 *
 * \param int amount
 * amount of times the ball will move
 * \return void
 *
 */

void move(int amount){
    for (int i = 0; i < amount;i++){
        updateBall();
        ball.x += ball.xSpeed;
        ball.y += ball.ySpeed;
        checkBounds();
    }
}
/*Clears the renderer
Draws the ball using the ball radius and ball location
small delay to see the movement better
Updates the renderer
*/
void updateBall(){
    //Clear Screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //Ball Draw
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int br = ball.radius;
    for(int i = -br; i <= br; i++){
        for(int j = -br; j <= br; j++){
                if(i * i + j * j <= br * br){
                    SDL_RenderDrawPoint(renderer, ball.x + i, ball.y + j);
                }
        }
    }
    SDL_Delay(10);
    SDL_RenderPresent(renderer);
}
/*Checks if the balls radius + the balls location is equal to the bounds of the machine
If it is it reverses the speed*/
void checkBounds(){
    //Game Section
    if(ball.radius + ball.x >= SCREEN_WIDTH || -ball.radius + ball.x <= 0){
        ball.xSpeed = -ball.xSpeed;
    }
    if(ball.radius + ball.y >= SCREEN_HEIGHT || -ball.radius + ball.y <= 0){
        ball.ySpeed = -ball.ySpeed;
    }
}


