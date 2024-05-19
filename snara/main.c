#include "mlx.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
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
	int		n_bit;
}	t_image;

typedef struct {
	size_t	i;
	size_t	len;
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
int	str_at(t_stream *str, ssize_t offset)
{
	if (0 <= str->i + offset && str->i + offset < str->len)
	{
		return str->ptr[str->i + offset];
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
	if (img->index == 0)
		exit(0);
	return 0;
}

int	key_handler(int key, t_image *img)
{
	(void)img;
	const int Macos_Escape = 53;
	if (key == XK_Escape || key == Macos_Escape)
		exit(0);
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
	const uint8_t *data = str->ptr;
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

int	seek_image(t_stream *str, t_point *size)
{
	for (;;)
	{
		if (!memcmp(&str->ptr[str->i], (uint8_t[]){0x6, 0x10, 0, 0}, 4))
		{
			int	x = str_at(str, -4);
			int	y = str_at(str, -2);
			*size = (t_point){x, y};
			printf("found %d * %d img %d bit \n", size->x, size->y, 5);
			str->i += 16;
			return 5;
		}
		if (!memcmp(&str->ptr[str->i], (uint8_t[]){0x4, 0x10, 0, 0}, 4))
		{
			int	x = str_at(str, -4);
			int	y = str_at(str, -2);
			*size = (t_point){x+(x&1), y}; // for some reason add 1 if odd
			printf("found %d * %d img %d bit \n", size->x, size->y, 8);
			str->i += 16;
			return 8;
		}
		if (ft_getc(str) == EOF)
		{
			printf("not found\n");
			return 0;
		}
	}
}

t_stream	str = {};

int	draw(t_image *img)
{
	if (img->n_bit == 8)
		for (size_t i=0; i<(size_t)(img->size.y * img->size.x); i+=1)
		{
			uint8_t		b = ft_getc(&str);
			uint8_t		g = ft_getc(&str);
			uint8_t		r = ft_getc(&str);
			size_t		n = img->bits_px / CHAR_BIT;
			img->addr[i*n+0] = b;
			img->addr[i*n+1] = g;
			img->addr[i*n+2] = r;
		}
	else if (img->n_bit == 5)
		for (size_t i=0; i<(size_t)(img->size.y * img->size.x); i+=1)
		{
			uint16_t	color = ft_getc(&str)<<0 | ft_getc(&str)<<8;
			uint8_t		r = (color>>10 & 0x1f)*8;
			uint8_t		g = (color>>5 & 0x1f)*8;
			uint8_t		b = (color>>0 & 0x1f)*8;
			size_t		n = img->bits_px / CHAR_BIT;
			img->addr[i*n+0] = b;
			img->addr[i*n+1] = g;
			img->addr[i*n+2] = r;
		}
	else
		return (-1);
	mlx_put_image_to_window(img->mlx, img->win, img->img, 0, 0);
	return (0);
}

#define MAX_IMAGE (100)

int	main(int ac, char *arg[])
{
	if (ac != 2)
		return fprintf(stderr, "usage: %s file\n", arg[0]), 0;
	FILE *fp = or_exit(fopen(arg[1], "r"), "failed to open file");
	str.len = fread(str.ptr, 1, BUFFER_SIZE, fp);
	fclose(fp);
	display_header(&str);

	void	*mlx = or_exit(mlx_init(), "mlx_init");
	static t_image	images[MAX_IMAGE] = {};
	for (size_t i=0; i < MAX_IMAGE; i++)
	{
		t_image	*img = &images[i];
		img->mlx = mlx;
		img->index = i;
		img->n_bit = seek_image(&str, &img->size);
		if (img->n_bit <= 0)
			break;
		char	n_str[20];
		img->win = or_exit(mlx_new_window(img->mlx, img->size.x, img->size.y, (snprintf(n_str, sizeof(n_str),  "%zu", i), n_str)), "mlx_new_window");
		img->img = or_exit(mlx_new_image(img->mlx, img->size.x, img->size.y), "mlx_new_image");
		img->addr = (uint8_t*)mlx_get_data_addr(img->img, &img->bits_px, &img->size_line, &img->endian);
		mlx_hook(img->win, DestroyNotify, StructureNotifyMask, &destroy_handler, img);
		mlx_key_hook(img->win, &key_handler, img);
		draw(img);
	}
	mlx_loop(mlx);
}

	__attribute__((destructor))
void	destructor()
{
	system("leaks -q retromfa");
}
