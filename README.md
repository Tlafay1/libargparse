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

# Example usage

```C
#include "argparse.h"

/**
 * This array of structures holds the informations about the args expected.
 * The last element of the array must be {0}.
 **/
static t_argo options[] = {
	{'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
	{'t', "test", "test", "Display a test", NO_ARG},
	{'o', "output", "output", "Specify output file", ONE_ARG},
	{'i', "input", "input", "Specify input file", INF_ARG},
	{0}
};

/**
 * This structure holds the informations about the parser.
 * The members are:
 * - options: the array of structures that holds the informations about the args expected.
 * - args_doc: a string that describes the non-option command-line arguments to use in the help menu.
 * - doc: a string that describes the program to the user in the help menu.
 **/

static t_argp argp = {
	.options = options,
	.args_doc = "INPUT_FILE OUTPUT_FILE",
	.doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
};

int main(int argc, char const *argv[])
{
	t_list *args_list = NULL;
    t_list *options_list = NULL;

	// Parse the arguments and store them in linked lists.
	parse_args(&argp, argv, &args_list, &options_list);
	t_argr *argr;

	// Loop through the options (e.g ./program -abc --option).
	while ((argr = get_next_option(&options_list)))
	{
		// Be careful, the option can be NULL.
		if (argr->option)
		{
			printf("sflag: %c, lflag: %s, name: %s\n", argr->option->sflag,
				   argr->option->lflag, argr->option->name);
		}
		// Be careful, the values can be NULL.
		if (argr->values)
			for (int i = 0; argr->values[i]; i++)
				printf("%s\n", argr->values[i]);
	}

	// Loop through the arguments (e.g ./program arg1 arg2 arg3).
	while ((argr = get_next_arg(&args_list)))
	{
		// Be careful, the option can be NULL.
		if (argr->option)
		{
			printf("sflag: %c, lflag: %s, name: %s\n", argr->option->sflag,
				   argr->option->lflag, argr->option->name);
		}
		// Be careful, the values can be NULL.
		if (argr->values)
			for (int i = 0; argr->values[i]; i++)
				printf("%s\n", argr->values[i]);
	}

	// IMPORTANT: don't forget to free the lists !
	free_args(args_list);
	free_args(options_list)

	return 0;
}
```

Output:

```bash
./test bonjour au revoir -i test -o test -v -v -v
sflag: i, lflag: input, name: input
test
sflag: o, lflag: output, name: output
test
sflag: v, lflag: verbose, name: verbose
sflag: v, lflag: verbose, name: verbose
sflag: v, lflag: verbose, name: verbose
bonjour
au
revoir
```
