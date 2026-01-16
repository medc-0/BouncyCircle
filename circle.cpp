#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>

#define WIDTH 900
#define HEIGHT 600

constexpr float GRAVITY = 0.5f;

struct Circle {
    float x, y;
    float radius;
    float vy;

    Circle(float x, float y, float radius)
    : x(x), y(y), radius(radius), vy(0.0f) {}
};

float randomize_num() 
{   
    return 0.2f + (rand() / (float)RAND_MAX) * (0.9f - 0.2f);
}

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

void fall_circle(Circle& c) 
{   
    c.vy += GRAVITY;
    c.y += c.vy;

    if (c.y + c.radius > HEIGHT) {
        c.y = HEIGHT - c.radius;
        c.vy = -c.vy * randomize_num();        
    }
}

int main() {
    srand(static_cast<unsigned>(time(NULL)));
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

    Circle circle(WIDTH / 2.0f, HEIGHT / 2.0f, 30.0f);

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
        fall_circle(circle);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}