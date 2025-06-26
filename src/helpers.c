#include <stdio.h>

#include "SDL2/SDL.h"

#include "services.h"
#include "constants.h"

double project_position_x(double z, double x) {
  double fov = FOV_X * (M_PI / 180.0);

  double factor = x / (tan(fov / 2) * z);

  double res = (WINDOW_W / 2) + (WINDOW_W / 2) * factor;

  return res;
}

double project_position_y(double z, double y) {
  double fov = FOV_Y * (M_PI / 180.0);

  double factor = y / (tan(fov / 2) * z);

  double res = (WINDOW_H / 2) - (WINDOW_H / 2) * factor;

  return res;
}

Screen_Point project_point(Vertice * point) {
  if (point->z > 0) {
    double x = project_position_x(point->z, point->x);
    double y = project_position_y(point->z, point->y);

    if (x >= -WINDOW_W && x <= 2 * WINDOW_W && y >= -WINDOW_H && y <= 2 * WINDOW_H ) {
      Screen_Point res = { .x = (int)x, .y = (int)y, .display_point = 1 };

      return res;
    } else {
      Screen_Point res = { .x = 0, .y = 0, .display_point = 0 };

      return res;
    }
  } else {
    Screen_Point res = { .x = 0, .y = 0, .display_point = 0 };

    return res;
  }
}

void rotate_vertice(Vertice * vertice, Cam_Rotation * cam_rotation) {
  double rad_rotation_hor = cam_rotation->rotation_x * (M_PI / 180.0);
  double rad_rotation_ver = cam_rotation->rotation_y * (M_PI / 180.0);

  double rotation_matrix_y_axis[3][3] = {
    {cos(rad_rotation_hor), 0, sin(rad_rotation_hor)},
    {0, 1, 0},
    {-sin(rad_rotation_hor), 0, cos(rad_rotation_hor)}
  };

  double rotation_matrix_x_axis[3][3] = {
    {1, 0, 0},
    {0, cos(rad_rotation_ver), -sin(rad_rotation_ver)},
    {0, sin(rad_rotation_ver), cos(rad_rotation_ver)}
  };

  // Vertical rotation around Y axis
  double new_x = rotation_matrix_y_axis[0][0] * vertice->x + rotation_matrix_y_axis[0][1] * vertice->y + rotation_matrix_y_axis[0][2] * vertice->z;
  double new_y = rotation_matrix_y_axis[1][0] * vertice->x + rotation_matrix_y_axis[1][1] * vertice->y + rotation_matrix_y_axis[1][2] * vertice->z;
  double new_z = rotation_matrix_y_axis[2][0] * vertice->x + rotation_matrix_y_axis[2][1] * vertice->y + rotation_matrix_y_axis[2][2] * vertice->z;

  // Horizontal rotation around X axis
  double temp_x = new_x;
  double temp_y = new_y;
  double temp_z = new_z;

  new_x = rotation_matrix_x_axis[0][0] * temp_x + rotation_matrix_x_axis[0][1] * temp_y + rotation_matrix_x_axis[0][2] * temp_z;
  new_y = rotation_matrix_x_axis[1][0] * temp_x + rotation_matrix_x_axis[1][1] * temp_y + rotation_matrix_x_axis[1][2] * temp_z;
  new_z = rotation_matrix_x_axis[2][0] * temp_x + rotation_matrix_x_axis[2][1] * temp_y + rotation_matrix_x_axis[2][2] * temp_z;

  vertice->x = new_x;
  vertice->y = new_y;
  vertice->z = new_z;
}
