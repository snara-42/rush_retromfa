#ifndef RETROMFA_H
#define RETROMFA_H

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "retromfa"

typedef struct s_mlx t_mlx;
typedef struct s_header t_header;
typedef enum e_event t_event;

enum e_event
{
  ON_KEYDOWN = 2,
  ON_KEYUP = 3,
  ON_MOUSEDOWN = 4,
  ON_MOUSEUP = 5,
  ON_MOUSEMOVE = 6,
  ON_EXPOSE = 12,
  ON_DESTROY = 17
};

struct s_mlx
{
  void *mlx;
  void *win;
  void *img;
  uint8_t *addr;
  int bits_per_pixel;
  int line_length;
  int endian;
};

struct s_header
{
  char MMF2[4];
  uint32_t n0[3];
  uint32_t n1;
  char *s1;
  uint32_t n2;
  char *s2;
  uint32_t n3;
  char *s3;
  uint32_t n4[14];
};

#endif