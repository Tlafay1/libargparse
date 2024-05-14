/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:27:31 by tlafay            #+#    #+#             */
/*   Updated: 2023/11/03 18:32:01 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparse.h"

typedef struct s_args
{
	t_list *args;
	t_list *options;
	t_list *args_original;
} t_args;

static int _unrecognized_option(char sflag, char *lflag, const char *progname, t_argp *argp)
{
	if (sflag == 'h' || (lflag && !ft_strncmp(lflag, "help", ft_strlen(lflag))))
	{
		help_args(argp, progname);
		return (1);
	}

	if (sflag)
		printf("%s: invalid option -- '%c'\n", progname, sflag);
	else
		printf("%s: unrecognized option '--%s'\n", progname, lflag);

	printf("Try '%s --help' for more information\n", progname);

	return (1);
}

static t_argo *_search_option(char sflag, char *lflag, t_argo *options)
{
	for (t_argo *option = options; option->sflag || option->lflag; option++)
	{
		if ((sflag && sflag == option->sflag) || (lflag && option->lflag && !ft_strncmp(lflag, option->lflag, ft_strlen(lflag))))
			return (option);
	}
	return (NULL);
}

static t_argo *_search_option_by_name(char *name, t_argo *options)
{
	for (t_argo *option = options; option->sflag || option->lflag; option++)
	{
		if (!ft_strncmp(name, option->name, ft_strlen(name)))
			return (option);
	}
	return (NULL);
}

static int _wrong_arguments_number(t_argo *option, int flagtype, const char *progname)
{
	char *message;

	if (option->argnum == ONE_ARG)
		message = "an";
	else
		message = "at least one";

	if (flagtype == SFLAG)
		printf("%s: option requires %s argument -- '%c'\n", progname, message, option->sflag);

	else
		printf("%s: option '%s' requires %s argument\n", progname, option->lflag, message);

	printf("Try '%s --help' for more information\n", progname);

	return (1);
}

static int _get_option_arguments(char ***args, t_argo *option,
								 char ***values, int flagtype, const char *progname)
{
	char **values_cp = *values;

	if (option->argnum == NO_ARG)
		values_cp = NULL;

	else if (option->argnum == ONE_ARG)
	{
		if (!*args || *args[0][0] == '-')
			return (_wrong_arguments_number(option, flagtype, progname));
		values_cp = (char **)malloc(2 * sizeof(char *));
		values_cp[0] = **args;
		values_cp[1] = NULL;
		(*args)++;
	}

	else
	{
		if (!**args)
			return (_wrong_arguments_number(option, flagtype, progname));
		int i = 0;
		for (; (*args)[i] && (ft_strlen((*args)[i]) <= 1 || (*args)[i][0] != '-'); i++)
			;
		if (!i)
			return (_wrong_arguments_number(option, flagtype, progname));
		values_cp = (char **)malloc((i + 1) * sizeof(char *));
		values_cp[i] = NULL;
		for (int j = 0; j < i; j++)
			values_cp[j] = (*args)[j];
		(*args) += i;
	}

	*values = values_cp;
	return (0);
}

static int _parse_short_option(char ***args, t_list **head, t_argo *options, const char *progname, t_argp *argp)
{
	t_argo *option;
	t_argr *ret;

	char *arg = (*args)[0] + 1;
	(*args)++;

	for (; *arg; arg++)
	{
		ret = (t_argr *)malloc(sizeof(t_argr));
		option = _search_option(*arg, NULL, options);

		if (!option)
		{
			free(ret);
			return (_unrecognized_option(*arg, NULL, progname, argp));
		}
		if (_get_option_arguments(args, option, &(ret->values), SFLAG, progname))
			return (1);
		ret->option = option;
		ft_lstadd_back(head, ft_lstnew(ret));
	}

	return (0);
}

static int _parse_long_option(char ***args, t_list **head, t_argo *options, const char *progname, t_argp *argp)
{
	t_argo *option;
	t_argr *ret;

	char *arg = (*args)[0] + 2;
	(*args)++;

	ret = (t_argr *)malloc(sizeof(t_argr));
	option = _search_option(0, arg, options);

	if (!option)
	{
		free(ret);
		return (_unrecognized_option(0, arg, progname, argp));
	}
	if (_get_option_arguments(args, option, &(ret->values), LFLAG, progname))
		return (1);
	ret->option = option;
	ft_lstadd_back(head, ft_lstnew(ret));

	return (0);
}

static int _parse_option(char ***args, t_list **head, t_argo *options, const char *progname, t_argp *argp)
{
	if (*(args[0][0] + 1) == '-')
		return _parse_long_option(args, head, options, progname, argp);
	return _parse_short_option(args, head, options, progname, argp);
}

/**
 * @brief Get the number of command line options.
 *
 * @param options_list The list containing the parsed options.
 * @return The number of command line options.
 */
int options_count(t_args *args)
{
	int count = 0;
	t_list *options_list = args->options;

	while (options_list)
	{
		if (((t_argr *)options_list->content)->option)
			count++;
		options_list = options_list->next;
	}
	return (count);
}

/**
 * @brief Get the number of command line arguments.
 *
 * @param args_list The list containing the parsed arguments.
 * @return The number of command line arguments.
 */
int args_count(t_args *args)
{
	int count = 0;
	t_list *args_list = args->args;

	while (args_list)
	{
		if (!((t_argr *)args_list->content)->option)
			count++;
		args_list = args_list->next;
	}
	return (count);
}

/**
 * @brief Free the structure when you are done using it.
 *
 * @param head The list containing the parsed arguments and options
 */
void free_args(t_args *args)
{
	t_list *tmp;
	t_argr *argr;

	if (!args)
		return;

	t_list *head = args->args_original;
	while (head)
	{
		tmp = head->next;
		argr = head->content;
		free(argr->values);
		free(argr);
		free(head);
		head = tmp;
	}
	free(args);
}

/**
 * @brief Get the next argument from the command line.
 * For example, if the options are `./program foo --test bar`,
 * the first call will return foo and the second bar.
 * @param args_list The structure containing the parsed arguments
 * @return The structure containing the args and options
 */
t_argr *get_next_arg(t_args *args)
{
	t_list **args_list = &args->args;

	while ((*args_list) && ((t_argr *)(*args_list)->content)->option)
		(*args_list) = (*args_list)->next;
	if (!(*args_list))
		return (NULL);
	t_argr *ret = (*args_list)->content;
	(*args_list) = (*args_list)->next;
	return (ret);
}

/**
 * @brief Get the next option from the command line.
 * For example, if the options are `./program --foo test --bar`,
 * the first call will return --foo and the second --bar.
 *
 * @param options_list The structure containing the parsed options
 * @return The structure containing the args and options
 */
t_argr *get_next_option(t_args *args)
{
	t_list **options_list = &args->options;

	while ((*options_list) && !((t_argr *)(*options_list)->content)->option)
		(*options_list) = (*options_list)->next;
	if (!(*options_list))
		return (NULL);
	t_argr *ret = (*options_list)->content;
	(*options_list) = (*options_list)->next;
	return (ret);
}

/**
 * @brief Print the help menu. The usual usage is to exit after.
 *
 * @param argp The options expected to be passed to the program.
 * @param prog_name The name of the program. Usually argv[0]
 */
void help_args(t_argp *argp, const char *prog_name)
{
	t_argo *options = argp->options;

	if (argp->args_doc)
		printf("Usage: %s %s\n", prog_name, argp->args_doc);

	if (argp->doc)
		printf("%s\n", argp->doc);

	for (; options->sflag || options->lflag; options++)
	{
		char *sflag = options->sflag ? ft_strjoin("-", (char[]){options->sflag, 0}) : "";
		char *lflag = options->lflag ? ft_strjoin("--", options->lflag) : "";
		if (options->lflag)
			printf("  %s, %-19s%s\n", sflag, lflag, options->help);
		else
			printf("  %s  %-13s%s\n", sflag, lflag, options->help);
		if (options->sflag)
			free(sflag);
		if (options->lflag)
			free(lflag);
	}
	if (_search_option_by_name("help", argp->options) == NULL)
		printf("  -%c, --%-17s%s\n", 'h', "help", "display this help and exit");
}

/**
 * @brief Parse the command line arguments.
 *
 * @param argp The options expected to be passed to the program.
 * @param argv The raw argv passed inside the main().
 * @param args_list The structure pointer to store the args. To be used in helper functions.
 * @param options_list The structure pointer to store the options. To be used in helper functions.
 * @return Error code, 0 if everything went fine, other for error.
 * 	It's recommended to exit the program in the case an error occured.
 */
int parse_args(t_argp *argp, const char *argv[], t_args **args)
{
	t_argr *ret;
	t_list *args_list = NULL;

	t_argo *options = argp->options;
	const char *progname = argv[0];
	argv++;
	for (; *argv;)
	{
		if ((*argv)[0] == '-' && (*argv)[1])
		{
			if (_parse_option((char ***)&argv, &args_list, options, progname, argp))
			{
				*args = NULL;
				return (1);
			}
		}
		else
		{
			ret = (t_argr *)malloc(sizeof(t_argr));
			ret->option = NULL;
			char **tmp = (char **)malloc(2 * sizeof(char *));
			tmp[0] = (char *)*argv;
			tmp[1] = NULL;
			ret->values = tmp;
			ft_lstadd_back(&args_list, ft_lstnew(ret));
			argv++;
		}
	}
	*args = (t_args *)malloc(sizeof(t_args));
	if (*args != NULL)
	{
		(*args)->args = args_list;
		(*args)->options = args_list;
		(*args)->args_original = args_list;
	}
	else
		return (1);
	return (0);
}
