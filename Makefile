
NAME		=	push

INCLUDES	=	-I includes

SRCPATH		=	srcs/

OBJPATH		=	objs/

SRCS		=	main.c\

SRC		=	$(addprefix $(SRCPATH), $(SRCS))

OBJ		=	$(patsubst $(SRCPATH)%.c, $(OBJPATH)%.o, $(SRC))

CFLAGS		+=	-W -Wall -Wextra -Werror -ggdb3 $(INCLUDES)

CC		=	gcc

all: $(NAME)

$(OBJ): | $(OBJPATH)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJPATH):
	mkdir -p $(OBJPATH)

clean:
	rm -rf $(OBJPATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(OBJPATH)%.o: $(SRCPATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re dir
