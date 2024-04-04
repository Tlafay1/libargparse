/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:28:05 by tlafay            #+#    #+#             */
/*   Updated: 2023/11/03 17:51:53 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdio.h>
#include <string.h>

#include "libft.h"

#define SFLAG 0
#define LFLAG 1

typedef enum e_argnum
{
	NO_ARG,
	ONE_ARG,
	INF_ARG
} e_argnum;

typedef struct s_arg_option
{
	const char sflag;
	const char *lflag;
	const char *name;
	const char *help;
	e_argnum argnum;
} t_argo;

typedef struct s_arg_return
{
	char **values;
	t_argo *option;
} t_argr;

typedef struct s_argp_parse
{
	t_argo *options;
	const char *args_doc;
	const char *doc;
} t_argp;

int parse_args(t_argp *argp, const char *argv[], t_list **head);
void help_args(t_argp *argp, const char *prog_name);
t_argr *get_next_arg(t_list *head);
t_argr *get_next_option(t_list *head);
void free_args(t_list *head);
int options_count(t_list *head);
int args_count(t_list *head);

#endif