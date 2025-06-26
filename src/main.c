#include <stdio.h>

#include "SDL2/SDL.h"

#include "helpers.h"
#include "services.h"

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = init_window();
  SDL_Renderer * renderer = init_renderer(window);

  run_engine(window, renderer);

  return 0;
}
