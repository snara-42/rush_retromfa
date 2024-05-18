#include <libc.h>
#include <stdbool.h>
#include <mlx.h>

#include "retromfa.h"

unsigned char *g_data;
size_t g_file_size;

void fatal_error(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(1);
}

unsigned char *read_file_to_buffer(const char *filename)
{
  struct stat st;
  if (stat(filename, &st) != 0)
    fatal_error("Failed to stat file");

  g_file_size = st.st_size;

  FILE *file = fopen(filename, "rb");
  if (!file)
    fatal_error("Failed to open file: %s", filename);

  unsigned char *buffer = malloc(g_file_size);
  if (!buffer)
    fatal_error("Failed to allocate memory for file");

  size_t bytes_read = fread(buffer, 1, g_file_size, file);
  if (bytes_read != g_file_size)
    fatal_error("Failed to read file: %s", filename);

  fclose(file);
  return buffer;
}

void display_header(const void *data)
{
  t_header h = {};
  size_t i = 0;

  memcpy(h.MMF2, &data[i], sizeof(h.MMF2));
  i += sizeof(h.MMF2);
  printf("%s\t", h.MMF2);

  memcpy(h.n0, &data[i], sizeof(h.n0));
  i += sizeof(h.n0);
  for (size_t i = 0; i < sizeof(h.n0) / sizeof(*h.n0); i++)
    printf("%u\t", h.n0[i]);
  printf("\n");

  memcpy(&h.n1, &data[i], sizeof(uint32_t));
  i += sizeof(uint32_t);
  h.s1 = (char *)&data[i];
  i += h.n1;
  printf("s1:[%.*s]\n", h.n1, h.s1);

  memcpy(&h.n2, &data[i], sizeof(uint32_t));
  i += sizeof(uint32_t);
  h.s2 = (char *)&data[i];
  i += h.n2;
  printf("s2:[%.*s]\n", h.n2, h.s2);

  memcpy(&h.n3, &data[i], sizeof(uint32_t));
  i += sizeof(uint32_t);
  h.s3 = (char *)&data[i];
  i += h.n3;
  printf("s3:[%.*s]\n", h.n3, h.s3);

  memcpy(h.n4, &data[i], sizeof(h.n4));
  i += sizeof(h.n4);
  for (size_t i = 0; i < sizeof(h.n4) / sizeof(*h.n4); i++)
    printf("%08x ", h.n4[i]);
  printf("\n\n");

  for (size_t j = 0; j < 256; ++j)
  {
    uint32_t px;
    memcpy(&px, &data[i], sizeof(uint32_t));
    i += sizeof(uint32_t);
    printf("%c%08x", " \n"[j % 8 == 0], px);
  }
  printf("\n\n");
}

int destroy_handler(void *param)
{
  (void)param;
  exit(0);
}

void draw(t_mlx *mlx)
{
  for (size_t i = 0; i < (HEIGHT * WIDTH); i += 3)
  {
    mlx->addr[i + i / 3 + 0] = g_data[i + 0];
    mlx->addr[i + i / 3 + 1] = g_data[i + 1];
    mlx->addr[i + i / 3 + 2] = g_data[i + 2];
  }
  mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
}

int main(int argc, char **argv)
{
  if (argc != 2)
    fatal_error("Usage: %s <mfa file>\n", argv[0]);

  g_data = read_file_to_buffer(argv[1]);
  printf("File size: %zu\n", g_file_size);

#ifdef DEBUG
  display_header(g_data);
#endif

  t_mlx mlx;
  memset(&mlx, 0, sizeof(mlx));
  mlx.mlx = mlx_init();
  if (mlx.mlx == NULL)
    fatal_error("Failed to initialize mlx\n");
  mlx.win = mlx_new_window(mlx.mlx, WIDTH, HEIGHT, WINDOW_TITLE);
  if (!mlx.win)
    fatal_error("Failed to create window");
  mlx.img = mlx_new_image(mlx.mlx, WIDTH, HEIGHT);
  if (!mlx.img)
    fatal_error("Failed to load image");
  mlx.addr = (uint8_t *)mlx_get_data_addr(mlx.img, &mlx.bits_per_pixel, &mlx.line_length, &mlx.endian);
  mlx_hook(mlx.win, ON_DESTROY, 1L << 17, &destroy_handler, NULL);
  draw(&mlx);
  mlx_loop(mlx.mlx);
  return 0;
}