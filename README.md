# Libargparse

## Library to parse command line arguments in C/C++.

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Tlafay1/libargparse?color=blueviolet" />
	<img alt="Number of lines of code" src="https://img.shields.io/tokei/lines/github/Tlafay1/libargparse?color=blueviolet" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/Tlafay1/libargparse?color=blue" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/Tlafay1/libargparse?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Tlafay1/libargparse?color=brightgreen" />
</p>

# Description

**This library is used for post common-core projects at 42, since we are not allowed to use the usual libraries and a lot of post CC projects require parsing command line arguments.**

# Installation

**This library requires the libft.** To use it with your main project, you'll need to do a few modifications to your makefiles:

- In your libft, create a shared library:

```makefile
gcc -shared -o libft.so -fPIC $(SRCS)
```
- Change your main project's Makefile to use the shared library:

```Makefile
$(CC) $(OBJDIR) ./libargparse/libargparse.a -o $(NAME) $(CFLAGS) -Llibft -lft -Wl,-R./libft
```

That's it !

# Usage

Coming soon TM ;)

---

:)
