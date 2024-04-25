AC_PREREQ([2.69])

AC_INIT([libargparse], [0.0.1], [tlafay@student.42lyon.fr])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_RANLIB
AC_PROG_CXX
AM_PROG_AR

AC_CHECK_LIB([gtest], [main])
# AC_CHECK_LIB([ft], [main])

# AC_CHECK_HEADER([include/argparse.h])
# AC_CHECK_HEADER([include/tests.hpp])

AC_SUBST([LIBFT_PATH], ["./libft"])

AC_CONFIG_FILES([
  Makefile
  include/Makefile
  lib/Makefile
  tests/Makefile
])

AC_OUTPUT
