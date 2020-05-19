#pragma once
#include <SDL.h>

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 32*8

bool middle{ false };
bool end{ false };

SDL_Event e;
SDL_Renderer* renderer;
SDL_Window* window;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
}

void updateScreen(uint8_t* screen) {
    for (int i = 0; i < WINDOW_WIDTH / 8; ++i) {
        for (int j = WINDOW_HEIGHT-1; j >= 0; --j) {
            //read individual bits
            int currentByte = j * WINDOW_WIDTH / 8 + i;
            
            currentByte == 256 * 32 / 2 ? middle = true : middle;
            currentByte == 256 * 32 ? end = true : end;

            uint8_t byte = screen[currentByte];
            for (int k = 7; k >= 0; --k) {
                if (((byte >> k) & 1) != 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, 8 * i + k, j);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, 8 * i + k, j);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}