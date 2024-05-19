#ifndef RETRO_MFA_H
#define RETRO_MFA_H
#include <stdbool.h>

typedef struct s_image
{
	int		bpp;
	int		sl;
	int		endian;
	int		height;
	int		width;
	void	*img;
	void	*addr;
}	t_image;

typedef struct s_mfa
{
	void		*mlx;
	void		*window;
    int         width;
    int         height;
	void        *addr;
	t_image		*image;
}	t_mfa;

typedef struct s_mfa_img
{
    unsigned int size;
    unsigned int rgb;
    unsigned int width;
    unsigned int height;
    bool        reverse;
}	t_mfa_img;


#endif
