#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> // Reikalinga lankų piešimui
#include <SDL2/SDL_ttf.h>            // Reikalinga gražiam šriftui
#include <switch.h>
#include <iostream>
#include <string>

// Ekrano raiškos nustatymai (Switch naudoja 1280x720)
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
    // Inicializuojame SDL2 grafinę aplinką
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Pomodoro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Įkeliame šriftą (failas turės būti tame pačiame aplanke)
    TTF_Font* font = TTF_OpenFont("romfs:/Roboto-Regular.ttf", 72);

    // Pomodoro kintamieji (25 minutės)
    const int TOTAL_TIME = 25 * 60; 
    int timeRemaining = TOTAL_TIME;
    bool isRunning = false;
    Uint32 lastTime = SDL_GetTicks();

    // Pagrindinis programos ciklas
    bool quit = false;
    while (!quit && appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) quit = true; // Plius mygtukas išjungia programą
        if (kDown & KEY_A) isRunning = !isRunning; // A mygtukas paleidžia/sustabdo

        // Laiko skaičiavimas kas sekundę
        Uint32 currentTime = SDL_GetTicks();
        if (isRunning && (currentTime - lastTime >= 1000)) {
            if (timeRemaining > 0) {
                timeRemaining--;
            } else {
                isRunning = false; // Laikas baigėsi
                // Čia galima įdėti kodą Switch pultelių vibracijai (hidVibrateController)
            }
            lastTime = currentTime;
        }

        // --- PIEŠIMAS EKRENE ---
        // 1. Profesionalus tamsiai mėlynas / pilkas fonas (#1a1a2e)
        SDL_SetRenderDrawColor(renderer, 26, 26, 46, 255);
        SDL_RenderClear(renderer);

        // Apskritimo centro koordinatės ir spindulys (ekrano centras)
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        int radius = 180;

        // 2. Nupiešiame pilką fono apskritimą (neaktyvų takelį)
        arcRGBA(renderer, centerX, centerY, radius, 0, 360, 60, 63, 85, 255);

        // 3. Progreso apskaičiavimas laipsniais (nuo 0 iki 360)
        double percentage = (double)timeRemaining / TOTAL_TIME;
        int endAngle = (int)(percentage * 360) - 90; // Atimam 90, kad prasidėtų nuo viršaus (12 valandos)

        // Nupiešiame ryškų, trumpėjantį progreso lanką aplink laikmatį (#00fff5 spalva)
        if (timeRemaining > 0) {
            // Naudojame storą liniją progreso ratui
            for (int offset = -4; offset <= 4; offset++) {
                arcRGBA(renderer, centerX, centerY, radius + offset, -90, endAngle, 0, 255, 245, 255);
            }
        }

        // 4. Teksto (skaičių) paruošimas ir piešimas centre
        int mins = timeRemaining / 60;
        int secs = timeRemaining % 60;
        char timeText[10];
        sprintf(timeText, "%02d:%02d", mins, secs);

        SDL_Color textColor = { 255, 255, 255, 255 }; // Balta spalva skaičiams
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, timeText, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = centerX - (textSurface->w / 2);
        textRect.y = centerY - (textSurface->h / 2);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Atlaisviname atmintį
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Atnaujiname ekraną
        SDL_RenderPresent(renderer);
    }

    // Programos uždarymas ir atminties išvalymas
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
