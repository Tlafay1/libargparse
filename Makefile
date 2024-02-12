CC := clang

LIB := lib/libargparse.a

CFLAGS := -Wall -Werror -Wextra

SRCS := argparse.c

TESTS := main.cpp

TESTS := $(addprefix tests/, ${TESTS})

INCLUDE := include/argparse.h

LIBFT_PATH := ../libft

OBJS := $(addprefix obj/, ${SRCS:.c=.o})

RM  = rm -f

all : $(LIB)

$(LIB) : $(OBJS) $(INCLUDE)
	@ar -rcs $@ $^

obj/%.o : src/%.c $(INCLUDE)
	@$(CC) -c -o $@ -I $(LIBFT_PATH) -I./include $< $(CFLAGS)

test: $(LIB)
	@$(CC) $(CFLAGS) \
		-Llib \
		-I./include \
		-pthread \
		-o tests/test \
		$(TESTS) \
		-largparse \
		-lgtest \
		-lft
	@./tests/test
	@$(RM) tests/test

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(LIB)

re : fclean all

.PHONY : all clean fclean re