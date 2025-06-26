#include <stdio.h>

#include "SDL2/SDL.h"

#include "helpers.h"
#include "constants.h"
#include "services.h"
#include "types.h"

Mesh init_cube() {
    Mesh cube;

    // Définition des points du cube
    Vertice v1 = {0, 0, 0};
    Vertice v2 = {1, 0, 0};
    Vertice v3 = {0, 1, 0};
    Vertice v4 = {1, 1, 0};
    Vertice v5 = {0, 0, 1};
    Vertice v6 = {1, 0, 1};
    Vertice v7 = {0, 1, 1};
    Vertice v8 = {1, 1, 1};

    cube.triangles_count = 12;

    // Face 1 (avant)
    cube.triangles[0].vertices[0] = v1;
    cube.triangles[0].vertices[1] = v3;
    cube.triangles[0].vertices[2] = v2;

    cube.triangles[1].vertices[0] = v2;
    cube.triangles[1].vertices[1] = v3;
    cube.triangles[1].vertices[2] = v4;

    // Face 2 (arrière)
    cube.triangles[2].vertices[0] = v5;
    cube.triangles[2].vertices[1] = v6;
    cube.triangles[2].vertices[2] = v7;

    cube.triangles[3].vertices[0] = v6;
    cube.triangles[3].vertices[1] = v8;
    cube.triangles[3].vertices[2] = v7;

    // Face 3 (gauche)
    cube.triangles[4].vertices[0] = v1;
    cube.triangles[4].vertices[1] = v5;
    cube.triangles[4].vertices[2] = v7;

    cube.triangles[5].vertices[0] = v1;
    cube.triangles[5].vertices[1] = v7;
    cube.triangles[5].vertices[2] = v3;

    // Face 4 (droite)
    cube.triangles[6].vertices[0] = v2;
    cube.triangles[6].vertices[1] = v4;
    cube.triangles[6].vertices[2] = v8;

    cube.triangles[7].vertices[0] = v2;
    cube.triangles[7].vertices[1] = v8;
    cube.triangles[7].vertices[2] = v6;

    // Face 5 (dessus)
    cube.triangles[8].vertices[0] = v3;
    cube.triangles[8].vertices[1] = v7;
    cube.triangles[8].vertices[2] = v4;

    cube.triangles[9].vertices[0] = v4;
    cube.triangles[9].vertices[1] = v7;
    cube.triangles[9].vertices[2] = v8;

    // Face 6 (dessous)
    cube.triangles[10].vertices[0] = v1;
    cube.triangles[10].vertices[1] = v2;
    cube.triangles[10].vertices[2] = v5;

    cube.triangles[11].vertices[0] = v2;
    cube.triangles[11].vertices[1] = v6;
    cube.triangles[11].vertices[2] = v5;

    // Position du cube
    cube.x = 1;
    cube.y = 1;
    cube.z = 5;

    // Taille du cube
    cube.size = 1;

    return cube;
}

SDL_Window * init_window() {
  SDL_Window * window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);

  if (window == NULL) {
    quit_engine(window, NULL, "Error create window.");
  }

  return window;
}

SDL_Renderer * init_renderer(SDL_Window * window) {
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    quit_engine(window, renderer, "Error create renderer.");
  }

  return renderer;
}

void run_engine(SDL_Window * window, SDL_Renderer * renderer) {
  int quit = 0;
  SDL_Event event;
  Cam_Rotation cam_rotation = { .rotation_x = CAM_ROTATION_X, .rotation_y = CAM_ROTATION_Y };

  Mesh meshes[MESHES_COUNT] = { init_cube() };

  const Uint8 * keystates = NULL;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    SDL_PumpEvents();
    
    keystates = SDL_GetKeyboardState(NULL);
    manage_inputs(keystates, meshes, &cam_rotation);

    set_draw_color(window, renderer, 255, 255, 255);

    for (int i = 0; i < MESHES_COUNT; i++) {
      render_mesh(window, renderer, &meshes[i], &cam_rotation);    
    }

    SDL_RenderPresent(renderer);

    set_draw_color(window, renderer, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Delay(10);
  }
}

void quit_engine(SDL_Window * window, SDL_Renderer * renderer, char * message) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  printf("Engine quit for the following reason :\n");
  printf("%s\n\n", message);
  printf("Error : \n%s\n", SDL_GetError());

  exit(1);
}

void set_draw_color(SDL_Window * window, SDL_Renderer * renderer, int r, int g, int b) {
  if (SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE) != 0) {
    quit_engine(window, renderer, "Error set draw color.");
  }
}

void draw_rectangle(SDL_Window * window, SDL_Renderer * renderer, int x, int y, int w, int h) {
  SDL_Rect rectangle = {.x = x, .y = y, .w = w, .h = h};

  if (SDL_RenderFillRect(renderer, &rectangle) != 0) {
    quit_engine(window, renderer, "Error draw rectangle");
  }
}

void draw_line(SDL_Window * window, SDL_Renderer * renderer, int x1, int y1, int x2, int y2) {
  if (SDL_RenderDrawLine(renderer, x1, y1, x2, y2) != 0) {
    quit_engine(window, renderer, "Error draw rectangle");
  }
}

void render_mesh(SDL_Window * window, SDL_Renderer * renderer, Mesh * mesh, Cam_Rotation * cam_rotation) {
  Screen_Point screen_points[3];
  for (int i = 0; i < mesh->triangles_count; i++) {
      Vertice transformed_vertices[3] = { mesh->triangles[i].vertices[0], mesh->triangles[i].vertices[1], mesh->triangles[i].vertices[2] };

      for (int j = 0; j < 3; j++) {
        transformed_vertices[j].x = transformed_vertices[j].x + mesh->x;
        transformed_vertices[j].y = transformed_vertices[j].y + mesh->y;
        transformed_vertices[j].z = transformed_vertices[j].z + mesh->z;

        rotate_vertice(&transformed_vertices[j], cam_rotation);

        screen_points[j] = project_point(&transformed_vertices[j]);
      }

      if (screen_points[0].display_point && screen_points[1].display_point) draw_line(window, renderer, screen_points[0].x, screen_points[0].y, screen_points[1].x, screen_points[1].y);
      if (screen_points[1].display_point && screen_points[2].display_point) draw_line(window, renderer, screen_points[1].x, screen_points[1].y, screen_points[2].x, screen_points[2].y);
      if (screen_points[2].display_point && screen_points[0].display_point) draw_line(window, renderer, screen_points[2].x, screen_points[2].y, screen_points[0].x, screen_points[0].y);
    }
}

void manage_inputs(const Uint8 * keystates, Mesh * meshes, Cam_Rotation * cam_rotation) {
  if (keystates[SDL_SCANCODE_W]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].z -= 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_S]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].z += 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_A]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].x += 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_D]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].x -= 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_SPACE]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].y -= 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_RSHIFT]) {
    for (int i = 0; i < MESHES_COUNT; i++) {
      meshes[i].y += 0.1;
    }
  }

  if (keystates[SDL_SCANCODE_RIGHT]) {
    if (cam_rotation->rotation_x <= 0.0) {
      cam_rotation->rotation_x = 360.0;
    } else {
      cam_rotation->rotation_x = cam_rotation->rotation_x - 1.0;
    }
  }

  if (keystates[SDL_SCANCODE_LEFT]) {
    if (cam_rotation->rotation_x >= 360.0) {
      cam_rotation->rotation_x = 0.0;
    } else {
      cam_rotation->rotation_x = cam_rotation->rotation_x + 1.0;
    }
  }

  if (keystates[SDL_SCANCODE_UP]) {
    if (cam_rotation->rotation_y < 180.0) {
      cam_rotation->rotation_y = cam_rotation->rotation_y + 1.0;
    }
  }

  if (keystates[SDL_SCANCODE_DOWN]) {
    if (cam_rotation->rotation_y > -180.0) {
      cam_rotation->rotation_y = cam_rotation->rotation_y - 1.0;
    }
  }
}
