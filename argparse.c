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

t_argo	*_g_argo = NULL;
char	*_g_progname = NULL;

static int _unrecognized_option(char sflag, char *lflag)
{
	if (sflag)
		printf("%s: invalid option -- '%c'\n", _g_progname, sflag);
	else
		printf("%s: unrecognized option '--%s'\n", _g_progname, lflag);

	printf("Try '%s --help' for more information\n", _g_progname);
	
	return (1);
}

static t_argo	*_search_option(char sflag, char *lflag)
{
	for (t_argo *options = _g_argo; options->sflag || options->lflag; options++)
	{
		if ((sflag && sflag == options->sflag)
			|| (lflag && !ft_strncmp(lflag, options->lflag, ft_strlen(lflag))))
			return (options);
	}
	return (NULL);
}

static int _wrong_arguments_number(t_argo *option, int flagtype)
{
	char	*message;

	if (option->argnum == ONE_ARG)
		message = "an";
	else
		message = "at least one";

	if (flagtype == SFLAG)
		printf("%s: option requires %s argument -- '%c'\n", _g_progname, message, option->sflag);

	else
		printf("%s: option '%s' requires %s argument\n", _g_progname, option->lflag, message);

	printf("Try '%s --help' for more information\n", _g_progname);

	return (1);
}

static int	_get_option_arguments(char ***args, t_argo *option,
	char ***values, int flagtype)
{
	char	**values_cp = *values;

	if (option->argnum == NO_ARG)
		values_cp = NULL;

	else if (option->argnum == ONE_ARG)
	{
		if (!*args)
			return (_wrong_arguments_number(option, flagtype));
		values_cp = (char **) malloc(2 * sizeof(char *));
		values_cp[0] = **args;
		values_cp[1] = NULL;
		(*args)++;
	}

	else
	{
		if (!**args)
			return (_wrong_arguments_number(option, flagtype));
		int i = 0;
		for (; (*args)[i] && (ft_strlen((*args)[i]) <= 1 || (*args)[i][0] != '-'); i++);
		values_cp = (char **) malloc((i + 1) * sizeof(char *));
		values_cp[i] = NULL;
		for (int j = 0; j < i; j++)
			values_cp[j] = (*args)[j];
		(*args) += i;
	}

	*values = values_cp;
	return (0);
}

static int _parse_short_option(char ***args, t_list **head)
{
	t_argo	*option;
	t_argr	*ret;

	char *arg = (*args)[0] + 1;
	(*args)++;

	ret = (t_argr *) malloc(sizeof(t_argr));
	for (; *arg; arg++)
	{
		option = _search_option(*arg, NULL);

		if (!option)
			return (_unrecognized_option(*arg, NULL));
		if (!*(arg + 1))
		{
			if (_get_option_arguments(args, option, &(ret->values), SFLAG))
				return (1);
		}
		else
			ret->values = NULL;
		ret->option = option;
		ft_lstadd_back(head, ft_lstnew(ret));
	}

	return (0);
}

static int _parse_long_option(char ***args, t_list **head)
{
	(void)head;
	(void)args;
	// char *arg = _g_progname + 2;
	return (0);
}

static int _parse_option(char ***args, t_list **head)
{
	if (*(args[0][0] + 1) == '-')
		return _parse_long_option(args, head);
	return _parse_short_option(args, head);
}

void	help_args(t_argp *argp, const char *prog_name)
{
	t_argo	*options = argp->options;

	if (argp->args_doc)
		printf("Usage: %s %s\n", prog_name, argp->args_doc);

	if (argp->doc)
		printf("%s\n", argp->doc);

	for (; options->sflag || options->lflag; options++)
		printf("  -%c, --%-17s%s\n", options->sflag, options->lflag, options->help);

	printf("  -%c, --%-17s%s\n", 'h', "help", "display this help and exit");
}

t_list	*parse_args(t_argp *argp, int argc, char const *argv[])
{
	t_list	*head;
	t_argr	*ret;
	char	**args;

	_g_argo = argp->options;
	_g_progname = (char *) argv[0];
	head = NULL;
	argv++;
	for (; *argv;)
	{
		if ((*argv)[0] == '-')
		{
			if (_parse_option((char ***)&argv, &head))
				return (NULL);
		}
		else
		{
			// segfault here
			ret->option = NULL;
			char **tmp = (char **)malloc(2 * sizeof(char *));
			tmp[0] = (char *) *argv;
			tmp[1] = NULL;
			ft_lstadd_back(&head, ft_lstnew(ret));
			argv++;
		}
		// else
	}

	t_list *tmp = head;
	while (tmp)
	{
		t_argr *test = tmp->content;
		printf("sflag: %c, lflag: %s, name: %s\n", test->option->sflag,
			test->option->lflag, test->option->name);
		for (int i = 0; test->values[i]; i++)
			printf("%s\n", test->values[i]);
		tmp = tmp->next;
	}
	return (head);
}
