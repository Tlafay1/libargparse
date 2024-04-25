AC_PREREQ([2.69])

AC_INIT([libargparse], [0.0.1], [tlafay@student.42lyon.fr])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_RANLIB
AM_PROG_AR

# AC_ARG_WITH(
#   [libft-path],
#   [
#     AS_HELP_STRING([--with-libft-path=PATH],
#     [Path to the libft library (default: ../libft)])
#   ]
# )

# AS_IF([test "x$with_libft_path" != "x"], [LIBFT_PATH=$with_libft_path], [LIBFT_PATH="../libft"])

AC_SUBST([LIBFT_PATH], ["./libft"])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  include/Makefile
  lib/Makefile
])

AC_OUTPUT
