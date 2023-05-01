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
# include "alloc_less_argv_parser.h"
# include "digests.h"
# include "block_getter.h"
# include "endianese.h"

typedef struct s_mode				t_mode;
typedef struct s_parser_state		t_parser_state;

struct								s_mode {
	char			*name;
	void			*run;
};

enum								e_ssl_error {
	OK,
	FT_SSL_INVALID_MODE,
	FT_SSL_INVALID_STRING,
	FT_SSL_CANT_READ_FILE
};

struct								s_parser_state {
	uint64_t		flags;
	size_t			processed;
	t_mode			mode;
	char			extra[64];
};

unsigned int						process_flag(
										const t_arg_parser_choice *choice,
										const char *arg,
										void *data);

unsigned int						process_string(
										const t_arg_parser_choice *choice,
										const char *arg,
										void *data);

unsigned int						process_mode(
										const t_arg_parser_choice *choice,
										const char *arg,
										void *data);

unsigned int						process_stdin(
										const t_arg_parser_choice *choice,
										const char *arg,
										void *data);

unsigned int						process_file(
										const t_arg_parser_choice *choice,
										const char *arg,
										void *data);

t_hash								md5(t_digest_block_getter *getter);

t_hash								sha256(t_digest_block_getter *getter);

int									digest_cleanup(void *state, int ret,
										int argc, t_csa argv);

unsigned int						process_io(const t_arg_parser_choice *self,
										const char *arg, void *state);

t_arg_parser_node					*base64_arguments(void);

int									base64_cleanup(void *state, int ret,
										const int argc, t_csa argv);

#endif
