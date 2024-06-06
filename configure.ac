AC_PREREQ([2.69])

AC_INIT([libargparse], m4_esyscmd([echo -n $VERSION]), [tlafay@student.42lyon.fr])

echo $VERSION

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# : ${CFLAGS=""}
# : ${CXXFLAGS=""}

AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR

AC_CHECK_LIB([gtest], [main])

AC_SUBST([LIBFT_PATH], ["./libft"])

AC_CONFIG_FILES([
  Makefile
  include/Makefile
  lib/Makefile
  tests/Makefile
])

AC_OUTPUT
