#include "mlx.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE (1<<26)
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
	size_t	index;
}	t_image;

typedef struct {
	size_t	i;
	ssize_t	len;
	uint8_t	ptr[BUFFER_SIZE];
}	t_stream;

int	ft_getc(t_stream *str)
{
	if (str->i < str->len)
	{
		return str->ptr[str->i++];
	}
	return EOF;
}
int	ft_ungetc(t_stream *str)
{
	if (str->i > 0)
	{
		return str->ptr[--str->i];
	}
	return EOF;
}
int	str_at(t_stream *str, size_t i)
{
	if (i < str->len)
	{
		return str->ptr[i];
	}
	return EOF;
}

void	*or_exit(void *ptr, const char *msg)
{
	if (ptr)
		return (ptr);
	fprintf(stderr, "fatal: %s\n", msg);
	exit(1);
}

int	destroy_handler(t_image *img)
{
	mlx_destroy_window(img->mlx, img->win);
	mlx_destroy_image(img->mlx, img->img);
	return 0;
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

void	display_header(t_stream *str)
{
	const void *data = str->ptr;
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
	str->i = i;

	for (size_t j=0; j<256; ++j) {
		uint32_t	px;
		memcpy(&px, &data[i], sizeof(uint32_t)); i+=sizeof(uint32_t);
		printf("%c%08x", " \n"[j%8==0], px);
	}
	printf("\n\n");
}

int	draw(t_image *img);

t_stream	str = {};

int	main(int ac, char *arg[])
{
	if (ac != 2)
		return fprintf(stderr, "usage: %s file\n", arg[0]), 0;
	FILE *fp = or_exit(fopen(arg[1], "r"), "failed to open file");
	// g_data = or_exit(calloc(1, BUFFER_SIZE), "malloc()");
	str.len = fread(str.ptr, 1, BUFFER_SIZE, fp);

	display_header(&str);

	{
		void	*mlx = or_exit(mlx_init(), "mlx_init");
		t_image images[20] = {};
		t_point size = (t_point){39, 120};
		for (size_t i=0; i<sizeof(images)/sizeof(t_image); i++)
		{
			t_image	*img = &images[i];
			img->mlx = mlx;
			img->index = i;
			img->size = size;
			img->win = or_exit(mlx_new_window(img->mlx, img->size.x, img->size.y, arg[1]), "mlx_new_window");
			img->img = or_exit(mlx_new_image(img->mlx, img->size.x, img->size.y), "mlx_new_image");
			img->addr = (uint8_t*)mlx_get_data_addr(img->img, &img->bits_px, &img->size_line, &img->endian);
			mlx_hook(img->win, DestroyNotify, StructureNotifyMask, &destroy_handler, img);
			draw(img);
		}
		//mlx_loop_hook(mlx.mlx, &draw, &mlx);
		mlx_loop(mlx);
	}
}

int	draw(t_image *img)
{
	for (;;)
	{
		if (!memcmp(&str.ptr[str.i], (uint8_t[]){img->size.x,0,img->size.y,0}, 4))
		{
			printf("size found\n");
			for (int i=0; i<16; i++) ft_getc(&str);
			break;
		}
		if (ft_getc(&str) == EOF)
		{
			printf("not found\n");
			return -1;
		}
	}
#if 0
	for (size_t y=0; y<(size_t)img->size.y; y++)
	{
		for (size_t x=0; x < (size_t)img->size.x; x++)
		{
			size_t	i = y * img->size.x + x;
			img->addr[i+i/3+4] = str_at(&str, i);
		}
	}
#endif
#if 0
	for (size_t i=0; i<(size_t)img->size.y*(size_t)img->size.x; i+=1)
	{
		size_t		n = img->bits_px/CHAR_BIT;
		img->addr[i*n+1] = ft_getc(&str);
		img->addr[i*n+2] = ft_getc(&str);
		img->addr[i*n+3] = ft_getc(&str);
	}
#elif 1
	for (size_t i=0; i<(size_t)img->size.y*(size_t)img->size.x; i+=1)
	{
		uint16_t	color = ft_getc(&str)<<0 | ft_getc(&str)<<8;
		uint8_t		r = (color>>11 & 0x1f)*8;
		uint8_t		g = (color>>5 & 0x1f)*8;
		uint8_t		b = (color>>0 & 0x1f)*8;
		size_t		n = img->bits_px/CHAR_BIT;
		img->addr[i*n+1] = r;
		img->addr[i*n+2] = g;
		img->addr[i*n+3] = b;
	}
#endif
	mlx_put_image_to_window(img->mlx, img->win, img->img, 0, 0);
	return (0);
}

	__attribute__((destructor))
void	destructor()
{
	system("leaks -q retromfa");
}
