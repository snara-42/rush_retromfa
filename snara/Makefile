NAME = retromfa
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -I$(MLXDIR)
LDFLAGS = -L$(MLXDIR) -lmlx -L/usr/X11/lib -lX11 -lXext
SRCS = main.c
OBJS = $(SRCS:.c=.o)
MLXDIR = ./minilibx-linux/

$(NAME): $(OBJS)
	make -C $(MLXDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: re
debug: CFLAGS = -Wall -Wextra -MMD -MP -g -fsanitize=address,undefined
d: debug
