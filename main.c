#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<stdio.h>
#include<stdbool.h>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 500;
int main(){
    //Initialized SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ball Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Should Hack it to stay up
    SDL_Event e; bool quit = false; while(quit == false){
            while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT) quit = true;}}


    //creates Ball Structure
    typedef struct {
        int x, y;
        int radius;
        int xSpeed, ySpeed;
    } Ball;
    Ball ball = {400, 300, 20, 5, 5};



    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


