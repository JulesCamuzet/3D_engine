#include "SDL2/SDL.h"

#include "types.h"

#ifndef HELPERS_H_
#define HELPERS_H_

double project_position_x(double z, double x);
double project_position_y(double z, double y);
Screen_Point project_point(Vertice * point);
void rotate_vertice(Vertice * vertice, Cam_Rotation * cam_rotation);

#endif
