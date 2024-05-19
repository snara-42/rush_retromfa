NAME = retromfa
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP $(INCFLAGS)
INCFLAGS = -I$(MLXPATH) -I/usr/X11/include 
SRCS = main.c
OBJS = $(SRCS:.c=.o)

ifeq ($(shell uname), Darwin)
MLXPATH = ./minilibx_opengl
#LDFLAGS = -L$(MLXPATH) -lmlx -framework OpenGL -framework AppKit -lz
LDFLAGS = $(MLXPATH)/*.o -framework OpenGL -framework AppKit -lz
else
MLXPATH = ./minilibx-linux
LDFLAGS = -L$(MLXPATH) -lmlx -L/usr/X11/lib -lX11 -lXext -lm -lbsd
#LDFLAGS = $(MLXPATH)/*.o    -L/usr/X11/lib -lX11 -lXext -lm -lbsd
endif


$(NAME): $(OBJS)
	make -C $(MLXPATH)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@ 

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: re
debug: CFLAGS = -Wall -Wextra -Wconversion -MMD -MP -g -fsanitize=address,undefined $(INCFLAGS)
d: debug

run: debug
	for f in ../MFA/*; { ./retromfa "$$f"; }
