#pragma once
#include <SDL.h>

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 32*8

SDL_Event e;
SDL_Renderer* renderer;
SDL_Window* window;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH - 32, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
}

void updateUpperHalf(uint8_t* screen) {
    for (int i = 0; i < WINDOW_WIDTH / 8; ++i) {
        for (int j = WINDOW_HEIGHT-1; j >= WINDOW_HEIGHT/2; --j) {
            //read individual bits
            int currentByte = j * WINDOW_WIDTH / 8 + i;
            uint8_t byte = screen[currentByte];
            for (int k = 7; k >= 0; --k) {
                if (((byte >> k) & 1) != 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, j, -(8 * i + k) + WINDOW_HEIGHT);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, j, -(8 * i + k) + WINDOW_HEIGHT);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void updateBottomHalf(uint8_t* screen) {
    for (int i = 0; i < WINDOW_WIDTH / 8; ++i) {
        for (int j = WINDOW_HEIGHT / 2 - 1; j >= 0; --j) {
            //read individual bits
            int currentByte = j * WINDOW_WIDTH / 8 + i;
            uint8_t byte = screen[currentByte];
            for (int k = 7; k >= 0; --k) {
                if (((byte >> k) & 1) != 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, j, -(8 * i + k) + WINDOW_HEIGHT);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, j, -(8 * i + k) + WINDOW_HEIGHT);
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