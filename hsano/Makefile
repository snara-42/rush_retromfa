NAME 	:= retromfa
LIB	:= ./lib/
OBJDIR	:= ./obj

LIBMLXDIR	:= $(LIB)/minilibx-linux
LIBMLXTAR	:= $(LIB)/minilibx-linux.tgz
LIBMLX		:= $(LIBMLXDIR)/libmlx.a

LIBFTDIR	:= $(LIB)/libft
LIBFTNAME	:= libft.a
LIBFT		:= $(LIBFTDIR)/$(LIBFTNAME)

SRCDIR	:= ./srcs/
SRC	:= main.c header.c utility.c image.c

SRCS	:= $(addprefix $(SRCDIR), $(SRC))
OBJS	:= $(SRCS:.c=.o)
OBJECTS	:= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
DEPS	:= $(OBJECTS:.o=.d)
ifeq ($(shell uname),Darwin)
INCS	:= ./include $(LIBFTDIR)/include $(LIBMLXDIR) /opt/X11/include
else
INCS	:= ./include $(LIBFTDIR)/include $(LIBMLXDIR)
endif

LIBDIRS	:= $(LIBFTDIR) $(LIBMLXDIR) 
IFLAGS	:= $(addprefix -I,$(INCS))
LFLAGS	:= $(addprefix -L,$(LIBDIRS))
LIBS	:= $(LIBFT) $(LIBMLX)

CC	:= cc
CFLAGS	:= -Wall -Wextra -Werror
ifeq ($(shell uname),Darwin)
LDFLAGS := $(IFLAGS) -L$(LIBFTDIR) -lft -lm -L/usr/X11R6/lib -lmlx -lXext -framework OpenGL -framework AppKit
else
LDFLAGS := $(IFLAGS) $(LFLAGS) -lft -lm -lmlx -lX11 -lXext
#LDFLAGS := $(IFLAGS) $(LFLAGS) -lft -lm -lmlx -lX11 -lXext -fsanitize=address  -fsanitize=undefined -fsanitize=leak -g3

endif

ifdef WITH_BONUS
DFLAGS  := -D BONUS=1
endif

all:	$(LIBMLX)	
	echo $(DELENTRY)
	@make -C $(LIBFTDIR)
	@make -C $(LIBMLXDIR)
	@make $(NAME)	

$(NAME)	:	$(OBJECTS) $(LIBS) 
		$(CC)  $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(DFLAGS) -o $@
		$(RM) $(DELENTRY:.c=.o)
		$(RM) $(DELENTRY:.c=.d)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) / $(*D)
	$(CC) $(CFLAGS) $(IFLAGS) $(BONUS_FLAG) $(DFLAGS) -c $< -MMD -MP -MF   $(OBJDIR)/$*.d  -o $@

$(LIBMLX) :
	tar zxvf $(LIBMLXTAR) -C $(LIB)

clean	:
			$(RM) $(OBJECTS)
			$(RM) $(DEPS)
			$(RM) $(DELENTRY:.c=.o)
			$(RM) $(DELENTRY:.c=.d)
			@make clean -C $(LIBFTDIR)
			@make clean -C $(LIBMLXDIR)

fclean	:	clean
			$(RM) $(NAME)
			$(RM) $(LIBS)

re	:		fclean all

bonus	:	
			@make WITH_BONUS=1

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

.PHONY: all clean fclean bonus re
