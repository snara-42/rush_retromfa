NAME := retromfa
CFLAGS := -Wall -Wextra -Werror -MMD -MP -I ./minilibx -fsanitize=address
LDFLAGS := -L ./minilibx -l mlx_Darwin -L/usr/X11R6/lib -lX11 -lXext -framework OpenGL -framework AppKit
SRCS := main.c
OBJS := $(SRCS:.c=.o)
DEPENDS := $(SRCS:.c=.d)
MLX := ./minilibx/

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(MLX)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.c.o: $(OBJS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(MLX) clean
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all

-include $(DEPENDS)