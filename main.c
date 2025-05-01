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
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include <SDL_ttf.h>

void move();
void checkBounds();
void updateAll();
void defaultPos();

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 500;

const int PADDLE_HEIGHT = 200;
const int LPADDLE_X = 100;
const int RPADDLE_X = 580;
const int PADDLE_SPEED = 5;

int leftScore = 0;
int rightScore = 0;
int scoreDetection = 0;

//creates Ball Structure
    typedef struct {
        int x, y;
        int radius;
        int xSpeed, ySpeed;
    } Ball;

//creates Paddle Structure
    typedef struct {
        int x, y;
        int width, length;
        int ySpeed;
    } Paddle;

Paddle leftPaddle = {LPADDLE_X, PADDLE_HEIGHT, 20, 100, PADDLE_SPEED};
Paddle rightPaddle = {RPADDLE_X, PADDLE_HEIGHT, 20, 100, PADDLE_SPEED};

Ball ball = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 5, 5};
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
TTF_Font* font = NULL;

int main(){
    //Seed for rand
    srand(time(NULL));
    //Initialized SDL
    SDL_Init(SDL_INIT_VIDEO);
    //Initialize the Text
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Ball Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Loads font
    font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 24);

//Makes the game end after someone gets 5 points
    while(leftScore != 5 && rightScore != 5){

        //Makes the ball go in a random direction

        int xrand = rand();
        int yrand = rand();

        ball.xSpeed = (rand() % 2 == 0) ? 5 : -5;
        ball.ySpeed = (rand() % 2 == 0) ? 5 : -5;

        SDL_Event e;
        //When someone scores it resets the screen
        while(scoreDetection != 1){
            //Detects the keyboard for any presses
            while(SDL_PollEvent(&e)){
                //detects if the application is clsoed
                if(e.type == SDL_QUIT){
                exit(0);
                }
            }
            //checks if an appropriate key was pressed
            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            //Checks to make sure the Paddle is not going to go out of bounds
            if(keystates[SDL_SCANCODE_W]){
                if(!(leftPaddle.y <= 0)){
                    leftPaddle.y -= leftPaddle.ySpeed;
                }
            }
            if(keystates[SDL_SCANCODE_S]){
                if(!(leftPaddle.y + 100 >= SCREEN_HEIGHT)){
                    leftPaddle.y += leftPaddle.ySpeed;
                }
            }
            if(keystates[SDL_SCANCODE_UP]){
                if(!(rightPaddle.y <= 0)){
                    rightPaddle.y -= rightPaddle.ySpeed;
                }
            }
            if(keystates[SDL_SCANCODE_DOWN]){
                if(!(rightPaddle.y + 100 >= SCREEN_HEIGHT)){
                    rightPaddle.y += rightPaddle.ySpeed;
                }
            }

            move();
        }
        SDL_Delay(1000);

       defaultPos();
       scoreDetection = 0;
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(100);
    //Should Hack it to stay up May need to comment
    //SDL_Event e; bool quit = false; while(quit == false){
     //       while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT) quit = true;}}

    SDL_DestroyTexture(texture);
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

void move(){

    updateAll();
    ball.x += ball.xSpeed;
    ball.y += ball.ySpeed;
    checkBounds();

}
/*Clears the renderer
Draws the ball using the ball radius and ball location
small delay to see the movement better
Updates the renderer
*/
void updateAll(){
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
    //Updates Left Paddle

    for(int i = leftPaddle.x; i <= leftPaddle.x + leftPaddle.width; i++){
        for(int j = leftPaddle.y; j <= leftPaddle.y + leftPaddle.length; j++){
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    //Updates Right Paddle
    for(int i = rightPaddle.x; i <= rightPaddle.x + rightPaddle.width; i++){
        for(int j = rightPaddle.y; j <= rightPaddle.y + rightPaddle.length; j++){
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }

    //Sets text color
        SDL_Color textColor = {255, 255, 255};
        char scoreText[50];

        sprintf(scoreText, "%d - %d", leftScore, rightScore);
        //Sets text
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        //Combines to surface
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect;
        textRect.x = SCREEN_WIDTH/2;
        textRect.y = 20;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_Delay(5);
    SDL_RenderPresent(renderer);
}
/** \brief
 *
 * Checks if the ball is going to bounce off the walls or Paddles
 *
 *
 *
 */

void checkBounds(){
    //Defines the edges of the ball
    int ballR = ball.radius + ball.x;
    int ballL = -ball.radius + ball.x;

    int ballT = -ball.radius + ball.y;
    int ballB = ball.radius + ball.y;


    //Detects if the ball hits the right or left side of the screen
    if(ballR >= SCREEN_WIDTH || -ball.radius + ball.x <= 0){
        if(ballR >= SCREEN_WIDTH){
            leftScore += 1;
        }
        else{
            rightScore += 1;
        }
        scoreDetection += 1;
    }
    //Detects if the ball hits the top or bottom of the screen
    else if(ballB >= SCREEN_HEIGHT || ballT <= 0){
        ball.ySpeed = -ball.ySpeed;
    }
    //Detects if the ball hits either paddle
    else{
        if(ballR >= rightPaddle.x && ballL <= rightPaddle.x + 20 && ballT <= rightPaddle.y + 100 && ballB >= rightPaddle.y){

            ball.xSpeed = -ball.xSpeed;
            ball.ySpeed = -ball.ySpeed;
        }
        if(ballL <= leftPaddle.x + 20 && ballR >= leftPaddle.x && ballT <= leftPaddle.y + 100 && ballB>= leftPaddle.y){

            ball.xSpeed = -ball.xSpeed;
            ball.ySpeed = -ball.ySpeed;
        }
    }



}
/** \brief Sets Ball and Paddles to starting positions
 *
 */
void defaultPos(){



    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT/2;
    ball.radius = 20;
    ball.xSpeed = 5;
    ball.ySpeed = 5;


    leftPaddle.x = LPADDLE_X;
    leftPaddle.y = PADDLE_HEIGHT;
    leftPaddle.width = 20;
    leftPaddle.length = 100;
    leftPaddle.ySpeed = PADDLE_SPEED;


    rightPaddle.x = RPADDLE_X;
    rightPaddle.y = PADDLE_HEIGHT;
    rightPaddle.width = 20;
    rightPaddle.length = 100;
    rightPaddle.ySpeed = PADDLE_SPEED;

    updateAll();
}


