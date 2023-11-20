CC = gcc

NAME = libargparse.a

CFLAGS = -Wall -Werror -Wextra -g

SRCS = argparse.c

INCLUDES = argparse.h

OBJS = ${SRCS:.c=.o}

RM  = rm -f 

all : $(NAME)

$(NAME) : $(OBJS) $(SRCS)
	@ar -rcs $(NAME) $(OBJS)

%.o : %.c $(INCLUDES)
	@$(CC) -c -I ../libft $< $(CFLAGS)

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re