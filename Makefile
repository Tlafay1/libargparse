CC := gcc

CC_TEST := g++

LIB := lib/libargparse.a

CFLAGS := -Wall -Werror -Wextra

SRCS := argparse.c

TESTS := main.cpp tests.cpp

TESTS := $(addprefix tests/, ${TESTS})

INCLUDE := include/argparse.h

LIBFT_PATH := ../libft

OBJS := $(addprefix obj/, ${SRCS:.c=.o})

RM  = rm -f

all : $(LIB)

$(LIBFT_PATH)/libft.a :
	$(MAKE) -C $(LIBFT_PATH)

$(LIB) : $(OBJS) $(INCLUDE) $(LIBFT_PATH)/libft.a
	ar -rcs $@ $^

obj/%.o : src/%.c $(INCLUDE)
	$(CC) -c -o $@ -I $(LIBFT_PATH) -I./include $< $(CFLAGS)

test: $(LIB)
	$(CC_TEST) $(CFLAGS) \
		-Llib \
		-L$(LIBFT_PATH) \
		-Wl,-R$(LIBFT_PATH) \
		-I./include \
		-I$(LIBFT_PATH) \
		-pthread \
		-o tests/test \
		$(TESTS) \
		-largparse \
		-lgtest \
		-lft
	./tests/test
	$(RM) tests/test

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(LIB)

re : fclean all

.PHONY : all clean fclean re
