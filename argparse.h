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
# define ARGPARSE_H

# include <stdio.h>
# include <string.h>

# include "libft.h"

# define SFLAG 0
# define LFLAG 1

typedef enum e_argnum
{
	NO_ARG,
	ONE_ARG,
	INF_ARG
}	e_argnum;

typedef struct s_arg_option
{
	char		sflag;
	char		*lflag;
	char		*name;
	char		*help;
	char		*type;
	e_argnum 	argnum;
}	t_argo;

typedef struct s_arg_return
{
	char	**values;
	t_argo	*option;
}	t_argr;

typedef struct s_argp_parse
{
	t_argo	*options;
	char	*args_doc;
	char	*doc;
}	t_argp;

t_list	*parse_args(t_argp *argp, int argc, char const *argv[]);
void	help_args(t_argp *argp, const char *prog_name);

#endif