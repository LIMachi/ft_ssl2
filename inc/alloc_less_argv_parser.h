/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_less_argv_parser.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_LESS_ARGV_PARSER_H
# define ALLOC_LESS_ARGV_PARSER_H

typedef const char *const				*t_csa;
typedef struct s_arg_parser_node		t_arg_parser_node;
typedef struct s_arg_parser_choice		t_arg_parser_choice;

typedef struct s_arg_parser_instance	t_arg_parser_instance;

typedef unsigned int					(*t_arg_parser_matching_choice_handler)(
	const t_arg_parser_choice *const choice, const char *const arg,
	void *data);

struct	s_arg_parser_node {
	const unsigned int				error;
	const t_arg_parser_node *const	next;
	unsigned int					count;
	t_arg_parser_choice				*choices;
};

struct	s_arg_parser_choice {
	const unsigned int						priority;
	const char								alias;
	const char *const						label;
	const int								take_arg;
	t_arg_parser_matching_choice_handler	on_match;
	const t_arg_parser_node					*next_if_match;
};

struct	s_arg_parser_instance {
	const t_arg_parser_node			*node;
	int								argc;
	t_csa							argv;
	int								arg;
	int								alias;
	unsigned int					err;
	void							*data;
};

int	parse_argv(int argc, t_csa argv, const t_arg_parser_node *root, void *data);

#endif
