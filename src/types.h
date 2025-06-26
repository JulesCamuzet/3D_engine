#ifndef TYPES_H_
#define TYPES_H_

typedef struct s_vertice {
  double x;
  double y;
  double z;
} Vertice;

typedef struct s_triangle {
  Vertice vertices[3];
} Triangle;

typedef struct s_mesh {
  Triangle triangles[100];
  double x;
  double y;
  double z;
  double size;
  int triangles_count;
} Mesh;

typedef struct s_screen_point {
  int x;
  int y;
  int display_point;
} Screen_Point;

typedef struct s_cam_rotation {
  double rotation_x;
  double rotation_y;
} Cam_Rotation;

#endif
