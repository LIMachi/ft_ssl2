/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <stddef.h>
# include <stdint.h>
# include "argument_parser.h"

typedef enum e_ssl_error			t_ssl_error;
typedef enum e_parser_input_type	t_parser_input_type;

typedef struct s_modes				t_modes;
typedef struct s_mode				t_mode;
typedef struct s_parser_state		t_parser_state;
typedef struct s_input				t_input;

struct								s_mode {
	char	*name;
	int		(*run)(t_parser_state *state, int argc, t_argvp argv);
};

struct								s_modes {
	size_t	mode_count;
	t_mode	*modes;
};

enum								e_ssl_error {
	OK,
	FT_SSL_INVALID_MODE,
	FT_SSL_INVALID_STRING,
	FT_SSL_CANT_READ_FILE
};

enum								e_parser_input_type {
	STDIN,
	INPUT_FILE,
	INPUT_STRING,
	OUTPUT_FILE
};

struct								s_input {
	t_parser_input_type	type;
	const char			*arg;
	size_t				length;
	const char			*data;
};

struct								s_parser_state {
	size_t		mode;
	uint64_t	flags;
	size_t		cinputs;
	t_input		*inpts;
};

uint64_t							flag(char c);

int									process_flag(const t_choice *self,
										t_consume_params params, int *error,
										void *state);

int									process_string(const t_choice *self,
										t_consume_params params, int *error,
										void *state);

int									process_mode(const t_choice *self,
										t_consume_params params, int *error,
										void *state);

void								get_remainder_files(t_parser_state *state,
										int argc, t_argvp argv);

t_modes								modes(void);

int									md5(t_parser_state *s, int argc,
										t_argvp argv);

int									sha256(t_parser_state *state, int argc,
										t_argvp argv);

char								*read_fd(int fd, size_t *size);

char								*read_file(const char *path, size_t *size);

int									debug_digests(t_parser_state *state);

#endif
