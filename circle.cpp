#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

#define WIDTH 900
#define HEIGHT 600

struct Circle {
    int x, y;
    float radius;

    Circle(int x, int y, float radius)
    : x(x), y(y), radius(radius) {}
};

int SDL_RenderFillCircle(SDL_Renderer * renderer, Circle circle)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = circle.radius;
    d = circle.radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, circle.x - offsety, circle.y + offsetx,
                                     circle.x + offsety, circle.y + offsetx);
        status += SDL_RenderDrawLine(renderer, circle.x - offsetx, circle.y + offsety,
                                     circle.x + offsetx, circle.y + offsety);
        status += SDL_RenderDrawLine(renderer, circle.x - offsetx, circle.y - offsety,
                                     circle.x + offsetx, circle.y - offsety);
        status += SDL_RenderDrawLine(renderer, circle.x - offsety, circle.y - offsetx,
                                     circle.x + offsety, circle.y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (circle.radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Bouncing Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Circle circle(WIDTH / 2, HEIGHT / 2, 30.0f);

    bool running = true;
    SDL_Event event;

    while (running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillCircle(renderer, circle);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}