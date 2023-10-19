#include <SDL2/SDL.h>
#include <iostream>
#include <optional>

#include "InputSystem.h"

class SomeInputContext : public InputContext {
  public:
    SomeInputContext() {
      bind(InputEvent(InputEvent::Type::KEY_PRESS, SDLK_w), [](){
        std::cout << "Pressing W" << std::endl;
      });
      bind(InputEvent(InputEvent::Type::KEY_PRESS, SDLK_UP), [](){
        std::cout << "Pressing UP" << std::endl;
      });
    }
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::optional<SDL_Window*> initWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return {};
    }

    SDL_Window* window = SDL_CreateWindow("This is a window!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return {};
    }

    return window;
}

int main(int argc, char* argv[]) {
    auto windowOpt = initWindow();
    if (!windowOpt.has_value()) {
        return 1;
    }

    SDL_Window* window = *windowOpt;

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    bool quit = false;
    SDL_Event e;

    auto inputManager = InputManager::GetInstance();
    SomeInputContext helloWorldContext;
    inputManager->setActiveContext(&helloWorldContext);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            inputManager->Update(e);
        }

        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


