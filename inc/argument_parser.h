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

typedef const char *const *const	t_argvp;

typedef struct s_consume_params		t_consume_params;

typedef struct s_choice				t_choice;
typedef struct s_node				t_node;

typedef int (*const					t_consumer)(const t_choice *self,
	t_consume_params params, int *error, void *state);

typedef struct s_parse_result		t_parse_result;
typedef struct s_parser				t_parser;

struct								s_consume_params {
	const char *const	next_arg;
	int					*jump_arg;
};

/**
* int (*const			consume)(const t_choice *self, t_consume_params params,
* 			int *error, void *state);
*/
struct								s_choice {
	char				alias;
	const char *const	name;
	t_consumer			consume;
	const t_node *const	next;
};

struct								s_node {
	const int				missing_error;
	const t_node *const		next;
	int						choice_count;
	const t_choice			*choices;
};

struct								s_parse_result {
	int			err;
	int			argi;
	int			erri;
};

struct								s_parser {
	int			argc;
	char		**argv;
	void		*state;
	int			arg;
	int			alias_index;
};

int									str_match(const char *str1,
										const char *str2);
t_parse_result						parse_argv(int argc, t_argvp argv,
										void *state, const t_node *node);

#endif
