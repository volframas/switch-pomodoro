#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <switch.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    SDL_Window* window = SDL_CreateWindow("Pomodoro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    const int TOTAL_TIME = 25 * 60; 
    int timeRemaining = TOTAL_TIME;
    bool isRunning = false;
    Uint32 lastTime = SDL_GetTicks();

    bool quit = false;
    while (!quit && appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) quit = true; 
        if (kDown & KEY_A) isRunning = !isRunning; 

        Uint32 currentTime = SDL_GetTicks();
        if (isRunning && (currentTime - lastTime >= 1000)) {
            if (timeRemaining > 0) {
                timeRemaining--;
            } else {
                isRunning = false; 
            }
            lastTime = currentTime;
        }

        // Tamsus fonas
        SDL_SetRenderDrawColor(renderer, 26, 26, 46, 255);
        SDL_RenderClear(renderer);

        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        int radius = 180;

        // Pilkas žiedas fone
        arcRGBA(renderer, centerX, centerY, radius, 0, 360, 60, 63, 85, 255);

        // Ryškus trumpėjantis progreso žiedas
        double percentage = (double)timeRemaining / TOTAL_TIME;
        int endAngle = (int)(percentage * 360) - 90; 

        if (timeRemaining > 0) {
            for (int offset = -4; offset <= 4; offset++) {
                arcRGBA(renderer, centerX, centerY, radius + offset, -90, endAngle, 0, 255, 245, 255);
            }
        }

        // PATAISYTA: Sukurtas simbolių masyvas, kad tilptų tekstas
        int mins = timeRemaining / 60;
        int secs = timeRemaining % 60;
        char timeText[16];
        sprintf(timeText, "%02d:%02d", mins, secs);

        // Teksto braižymas centre
        stringRGBA(renderer, centerX - 20, centerY - 5, timeText, 255, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
