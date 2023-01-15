/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_parser.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGUMENT_PARSER_H
# define ARGUMENT_PARSER_H

# include <stddef.h>

typedef const char *const *const	t_argvp;

typedef struct s_choice				t_choice;
typedef struct s_node				t_node;

typedef struct s_parse_result		t_parse_result;
typedef struct s_parser				t_parser;

struct								s_choice {
	char				alias;
	const char *const	name;
	int (*const			callback)(const t_choice *self, t_parse_result *out,
			const char *arg, void *state);
	const t_node *const	next;
};

struct								s_node {
	const int				missing_error;
	const t_node *const		next;
	size_t					choice_count;
	const t_choice			*choices;
};

struct								s_parse_result {
	int			err;
	int			argi;
};

struct								s_parser {
	int			argc;
	t_argvp		argv;
	void *const	state;
};

int									str_match(const char *str1,
										const char *str2);
t_parse_result						parse(t_parser parser, const t_node *node);
t_parser							create_parser(int argc, t_argvp argv,
										void *state);

#endif
