NAME = retromfa
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -I$(MLXPATH)
SRCS = main.c
OBJS = $(SRCS:.c=.o)

ifeq ($(shell uname), Darwin)
MLXPATH = ./minilibx_opengl
LDFLAGS =  -L$(MLXPATH) -lmlx -framework OpenGL -framework AppKit
else
MLXPATH = ./minilibx-linux
LDFLAGS = x -L$(MLXPATH) -lmlx -I/usr/X11/include -L/usr/X11/lib -lX11 -lXext -lm -lbsd
endif


$(NAME): $(OBJS)
	make -C $(MLXPATH)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: re
debug: CFLAGS = -Wall -Wextra -MMD -MP -g -fsanitize=address,undefined -I$(MLXPATH)
d: debug

run: debug
	for f in ../MFA/*; { ./retromfa "$$f"; }
