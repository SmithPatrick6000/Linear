/**
*   File: main.c (may change name)
*   Author: Patrick Smith
*   Last Updated: 4/31/2025
*   Purpose: Simple SDL2 Pong Game
*
*   Description:
*   This program makes a simple Pong game
*   Add more info later
*
*   Dependencies:
*   - SDL2 library
*   - SDL_ttf library
*   - SDL_image library
*
*   Compile With:
*   Find this out I think gcc
*
*   Future plans:
*   Change the score detection from an into to bool.
*   Make the score Centered
*   Fix Bug where the ball gets stuck inside the Paddle
*
*   Find the optimal speed for ball and paddle
*   Optional: Make the ball speed up the longer its been since someone has socred
*             Make the messages look better/ flash on screen
*             Make window size editable
*             Make the Ball bounce more crazy
**/

#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include <SDL_ttf.h>
#include<SDL_image.h>

//Declares all functions used later
void move();
void checkBounds();
void updateAll();
void defaultPos();
void screenImage(char str[]);
//Sets Constant Screen Width and Height
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;
const int BALL_SPEED = 5;
//Sets Paddle Constants
const int PADDLE_HEIGHT = 200;
const int LPADDLE_X = 100;
const int RPADDLE_X = SCREEN_WIDTH - 120;
const int PADDLE_SPEED = 5;
//Sets the score and core detection
int leftScore = 0;
int rightScore = 0;
int scoreDetection = 0;
//Creates String to hold filenames
char filename[50];
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
//Creates left and Right Paddle and gives a location
Paddle leftPaddle = {LPADDLE_X, PADDLE_HEIGHT, 20, 100, PADDLE_SPEED};
Paddle rightPaddle = {RPADDLE_X, PADDLE_HEIGHT, 20, 100, PADDLE_SPEED};
//Creates ball and gives a location
Ball ball = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, BALL_SPEED, BALL_SPEED};

//Creates the SDL renderer, texture, and font
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
TTF_Font* font = NULL;
SDL_Surface* surface = NULL;

int main(){
    //Seed for rand
    srand(time(NULL));
    //Initialized SDL
    SDL_Init(SDL_INIT_VIDEO);
    //Initialize the Text
    TTF_Init();
    //Initialize Img
    IMG_Init(IMG_INIT_PNG);

    //Creates the window that the game uses
    SDL_Window* window = SDL_CreateWindow("Ball Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //Makes it so the renderer renders things inside of the window we created
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Loads font
    font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 24);

//Makes the game end after someone gets 5 points
    while(leftScore != 5 && rightScore != 5){

        //Makes the ball go in a random direction at the start of each round

        int xrand = rand();
        int yrand = rand();

        ball.xSpeed = (rand() % 2 == 0) ? ball.xSpeed : -ball.xSpeed;
        ball.ySpeed = (rand() % 2 == 0) ? ball.ySpeed : -ball.ySpeed;

        //Creates the SDL Event we will use for Key presses
        SDL_Event e;
        //Runs the game until someonescores
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
        //Gives a 1 second delay between each round
        SDL_Delay(1000);
        // resets the screen and sets score detection to 0
       defaultPos();
       scoreDetection = 0;
    }

    //Clears the Screen to White after the game has been won and displays a message
    if(leftScore == 5){
        filename[0] = '\0';
        strcpy(filename, "assets/images/Player1Wins.png");
        screenImage(filename);
    }else{
        filename[0] = '\0';
        strcpy(filename, "assets/images/Player2Wins.png");
        screenImage(filename);
    }

    SDL_Delay(1000);
    //Should Hack it to stay up May need to comment
    //SDL_Event e; bool quit = false; while(quit == false){
     //       while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT) quit = true;}}

    //Destroys all SDL things that were created
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
/** \brief Moves the ball a specified amount of times by adding the speed
 *  (xSpeed / ySpeed) to the balls x or y position
 *
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
Draws Paddles at new location
Updates score
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
            filename[0] = '\0';
            strcpy(filename, "assets/images/Player1Scores.png");
            screenImage(filename);

        }
        else{
            rightScore += 1;
            filename[0] = '\0';
            strcpy(filename, "assets/images/Player2Scores.png");
            screenImage(filename);
        }
        scoreDetection += 1;
    }
    //Detects if the ball hits the top or bottom of the screen
    else if(ballB >= SCREEN_HEIGHT || ballT <= 0){
        ball.ySpeed = -ball.ySpeed;
    }
    //Detects if the ball hits either paddle
    //Makes the ball move according to what the paddle that hits it is doing
    else{
        //Tracks the keystates
        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if(ballR >= rightPaddle.x && ballL <= rightPaddle.x + 20 && ballT <= rightPaddle.y + 100 && ballB >= rightPaddle.y){
            //If the paddle is moving down the ball will also move down after hitting it
            if(keystates[SDL_SCANCODE_DOWN]){
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = abs(ball.ySpeed);
                //If the paddle is moving up the ball will go up
            }else if(keystates[SDL_SCANCODE_UP]){
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = -abs(ball.ySpeed);
                //If the paddle is not moving the direction of the ball is reversed
            }else{
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = -ball.ySpeed;
            }
        }
        if(ballL <= leftPaddle.x + 20 && ballR >= leftPaddle.x && ballT <= leftPaddle.y + 100 && ballB>= leftPaddle.y){
            //If the paddle is moving down the ball will also move down after hitting it
            if(keystates[SDL_SCANCODE_S]){
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = abs(ball.ySpeed);
                //If the paddle is moving up the ball will go up
            }else if(keystates[SDL_SCANCODE_W]){
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = -abs(ball.ySpeed);
                //If the paddle is not moving the direction of the ball is reversed
            }else{
                ball.xSpeed = -ball.xSpeed;
                ball.ySpeed = -ball.ySpeed;
            }
        }
    }



}
/** \brief Sets Ball and Paddles to starting positions
 *
 */
void defaultPos(){


    //Sets ball to default position
    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT/2;
    ball.radius = 20;
    ball.xSpeed = BALL_SPEED;
    ball.ySpeed = BALL_SPEED;

    //Sets left Paddle to default position
    leftPaddle.x = LPADDLE_X;
    leftPaddle.y = PADDLE_HEIGHT;
    leftPaddle.width = 20;
    leftPaddle.length = 100;
    leftPaddle.ySpeed = PADDLE_SPEED;

    //Sets right Paddle to default position
    rightPaddle.x = RPADDLE_X;
    rightPaddle.y = PADDLE_HEIGHT;
    rightPaddle.width = 20;
    rightPaddle.length = 100;
    rightPaddle.ySpeed = PADDLE_SPEED;

    //Sets all things to new position
    updateAll();
}
/** \brief Displays the image file from the file directory that is input
 *
 * \param char str[50]  Is basically a string
 *
 */

void screenImage(char str[50]){
    //Sets Screen White
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    //Sets image to inputted string path
    surface = IMG_Load(str);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    //Sets the Area the img will be shown in and puts it into the renderer
    SDL_Rect destRect = {50, SCREEN_HEIGHT/4, SCREEN_WIDTH-100, 150};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
}

