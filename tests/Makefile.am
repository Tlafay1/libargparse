check_PROGRAMS = test
test_SOURCES = main.cpp tests.cpp
test_CPPFLAGS = -Wall -Werror -Wextra -I$(top_srcdir)/include -I$(top_srcdir)/$(LIBFT_PATH)
test_LDFLAGS = -L$(top_srcdir)/lib -L$(top_srcdir)/$(LIBFT_PATH) -Wl,-R$(top_srcdir)/$(LIBFT_PATH) -pthread
test_LDADD = -largparse -lgtest -lft
CLEANFILES = test

check: libft all
	./test

.PHONY: libft
libft:
	$(MAKE) -C $(top_srcdir)/$(LIBFT_PATH)
