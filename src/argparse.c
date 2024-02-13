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
		if ((sflag && sflag == option->sflag) || (lflag && !ft_strncmp(lflag, option->lflag, ft_strlen(lflag))))
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
			return (_unrecognized_option(*arg, NULL, progname, argp));
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
		return (_unrecognized_option(0, arg, progname, argp));
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

int options_count(t_list *head)
{
	int count = 0;

	while (head)
	{
		if (((t_argr *)head->content)->option)
			count++;
		head = head->next;
	}
	return (count);
}

int args_count(t_list *head)
{
	int count = 0;

	while (head)
	{
		if (!((t_argr *)head->content)->option)
			count++;
		head = head->next;
	}
	return (count);
}

t_argr *get_next_arg(t_list *head)
{
	static t_list *current = NULL;
	static int end = 0;

	if (end || !head)
		return (NULL);

	if (!current)
		current = head;

	while (current && ((t_argr *)current->content)->option)
		current = current->next;
	if (!current)
	{
		end = 1;
		return (NULL);
	}
	t_argr *ret = current->content;
	current = current->next;
	if (!current)
		end = 1;
	return (ret);
}

void free_args(t_list *head)
{
	t_list *tmp;
	t_argr *argr;

	while (head)
	{
		tmp = head->next;
		argr = head->content;
		free(argr->values);
		free(argr);
		free(head);
		head = tmp;
	}
}

t_argr *get_next_option(t_list *head)
{
	static t_list *current = NULL;
	static int end = 0;

	if (end || !head)
		return (NULL);

	if (!current)
		current = head;

	while (current && !((t_argr *)current->content)->option)
		current = current->next;
	if (!current)
	{
		end = 1;
		return (NULL);
	}
	t_argr *ret = current->content;
	current = current->next;
	if (!current)
		end = 1;
	return (ret);
}

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
	}
	if (_search_option_by_name("help", argp->options) == NULL)
		printf("  -%c, --%-17s%s\n", 'h', "help", "display this help and exit");
}

t_list *parse_args(t_argp *argp, int argc, char const *argv[])
{
	t_list *head;
	t_argr *ret;

	(void)argc;
	t_argo *options = argp->options;
	const char *progname = argv[0];
	head = NULL;
	argv++;
	for (; *argv;)
	{
		if ((*argv)[0] == '-' && (*argv)[1])
		{
			if (_parse_option((char ***)&argv, &head, options, progname, argp))
				return (NULL);
		}
		else
		{
			ret = (t_argr *)malloc(sizeof(t_argr));
			ret->option = NULL;
			char **tmp = (char **)malloc(2 * sizeof(char *));
			tmp[0] = (char *)*argv;
			tmp[1] = NULL;
			ret->values = tmp;
			ft_lstadd_back(&head, ft_lstnew(ret));
			argv++;
		}
	}

	return (head);
}
