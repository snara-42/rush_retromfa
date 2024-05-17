#include "mlx.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <string.h>
#include <unistd.h>

#define SIZE (1<<26)
typedef struct {
	int	x;
	int	y;
}	t_point;

typedef struct {
	void	*mlx;
	void	*win;
	void	*img;
	uint8_t	*addr;
	int		bits_px;
	int		size_line;
	int		endian;
	t_point	size;
}	t_mlx;

void	*or_exit(void *ptr, const char *msg)
{
	if (ptr)
		return (ptr);
	fprintf(stderr, "fatal: %s\n", msg);
	exit(1);
}

int	destroy_handler(void *param)
{
	(void)param;
	exit(0);
}

typedef struct {
	char		MMF2[4];
	uint32_t	n0[3];
	uint32_t	n1;
	char		*s1;
	uint32_t	n2;
	char		*s2;
	uint32_t	n3;
	char		*s3;
	uint32_t	n4[14];
}	t_header;

void	display_header(const void *data)
{
	t_header	h = {};
	size_t		i = 0;

	memcpy(h.MMF2, &data[i], sizeof(h.MMF2)); i+=sizeof(h.MMF2);
	printf("%s\t", h.MMF2);

	memcpy(h.n0, &data[i], sizeof(h.n0)); i+=sizeof(h.n0);
	for (size_t i=0; i<sizeof(h.n0)/sizeof(*h.n0); i++) printf("%u\t", h.n0[i]);
	printf("\n");

	memcpy(&h.n1, &data[i], sizeof(uint32_t)); i+=sizeof(uint32_t);
	h.s1 = (char*)&data[i]; i+=h.n1;
	printf("s1:[%.*s]\n", h.n1, h.s1);

	memcpy(&h.n2, &data[i], sizeof(uint32_t)); i+=sizeof(uint32_t);
	h.s2 = (char*)&data[i]; i+=h.n2;
	printf("s2:[%.*s]\n", h.n2, h.s2);

	memcpy(&h.n3, &data[i], sizeof(uint32_t)); i+=sizeof(uint32_t);
	h.s3 = (char*)&data[i]; i+=h.n3;
	printf("s3:[%.*s]\n", h.n3, h.s3);

	memcpy(h.n4, &data[i], sizeof(h.n4)); i+=sizeof(h.n4);
	for (size_t i=0; i<sizeof(h.n4)/sizeof(*h.n4); i++) printf("%08x ", h.n4[i]);
	printf("\n\n");

	for (size_t j=0; j<256; ++j) {
		uint32_t	px;
		memcpy(&px, &data[i], sizeof(uint32_t)); i+=sizeof(uint32_t);
		printf("%c%08x", " \n"[j%8==0], px);
	}
	printf("\n\n");
}

int	draw(t_mlx *mlx);

uint8_t	*g_data;

int	main(int ac, char *arg[])
{
	if (ac != 2)
		return fprintf(stderr, "usage: %s file\n", arg[0]), 0;
	FILE *fp = or_exit(fopen(arg[1], "r"), "failed to open file");
	g_data = or_exit(calloc(1, SIZE), "malloc()");
	ssize_t	len = fread(g_data, 1, SIZE, fp);
	(void)len;

	display_header(g_data);

	{
		t_mlx	mlx = {};
		mlx.size = (t_point){64, 1024};
		mlx.mlx = or_exit(mlx_init(), "mlx_init");
		mlx.win = or_exit(mlx_new_window(mlx.mlx, mlx.size.x, mlx.size.y, arg[1]), "mlx_new_window");
		mlx.img = or_exit(mlx_new_image(mlx.mlx, mlx.size.x, mlx.size.y), "mlx_new_image");
		mlx.addr = (uint8_t*)mlx_get_data_addr(mlx.img, &mlx.bits_px, &mlx.size_line, &mlx.endian);

		mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &destroy_handler, NULL);
		//mlx_loop_hook(mlx.mlx, &draw, &mlx);
		draw(&mlx);
		mlx_loop(mlx.mlx);
	}
}

int	draw(t_mlx *mlx)
{
#if 0
		for (size_t y=0; y<(size_t)mlx->size.y; y++)
		{
			for (size_t x=0; x < (size_t)mlx->size.x; x++)
			{
				size_t	i = y * mlx->size.x + x;
				mlx->addr[i+i/3+4] = g_data[i];
			}
		}
#elif 1
		for (size_t i=0; i<(size_t)mlx->size.y*(size_t)mlx->size.x; i+=3)
		{
			mlx->addr[i+i/3+0] = g_data[i+0];
			mlx->addr[i+i/3+1] = g_data[i+1];
			mlx->addr[i+i/3+2] = g_data[i+2];
		}
#else
		for (size_t i=0; i<(size_t)mlx->size.y*(size_t)mlx->size.x; i+=2)
		{
			uint16_t	color = g_data[i+0]<<8 | g_data[i+1];
			mlx->addr[i+i/3+0] = (color>>10 & 0x1f)*8;
			mlx->addr[i+i/3+1] = (color>>5 & 0x1f)*8;
			mlx->addr[i+i/3+2] = (color>>0 & 0x1f)*8;
		}
#endif
		mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
	return (0);
}

__attribute__((destructor))
void	destructor()
{
	system("leaks -q retromfa");
}
