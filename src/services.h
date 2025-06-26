#include "SDL2/SDL.h"

#include "types.h"

#ifndef SERVICES_H_
#define SERVICES_H_

// Init
SDL_Window * init_window();
SDL_Renderer * init_renderer(SDL_Window * window);
Mesh init_cube();

// Engine
void run_engine(SDL_Window * window, SDL_Renderer * renderer);
void quit_engine(SDL_Window * window, SDL_Renderer * renderer, char * message);

// Renderer controllers
void set_draw_color(SDL_Window * window, SDL_Renderer * renderer, int r, int g, int b);
void draw_rectangle(SDL_Window * window, SDL_Renderer * renderer, int x, int y, int w, int h);
void draw_line(SDL_Window * window, SDL_Renderer * renderer, int x1, int y1, int x2, int y2);

// Inputs
void manage_inputs(const Uint8 * keystates, Mesh * meshes, Cam_Rotation * cam_rotation);

// Render
void render_mesh(SDL_Window * window, SDL_Renderer * renderer, Mesh * mesh, Cam_Rotation * cam_rotation);

#endif
