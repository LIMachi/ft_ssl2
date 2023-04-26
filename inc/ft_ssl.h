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

typedef struct s_modes				t_modes;
typedef struct s_mode				t_mode;
typedef struct s_parser_state		t_parser_state;
typedef struct s_input				t_input;
typedef enum e_mode_category		t_mode_category;

typedef t_hash						(*t_digest_run)(t_digest_block_getter *);

enum								e_mode_category {
	DIGEST
};

struct								s_mode {
	char			*name;
	t_mode_category	category;
	void			*run;
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

struct								s_parser_state {
	size_t		mode;
	uint64_t	flags;
	size_t		processed;
	void		*internal;
};

unsigned int						process_flag(
										const t_arg_parser_choice *const choice,
										const char *const arg,
										void *data);

unsigned int						process_string(
										const t_arg_parser_choice *const choice,
										const char *const arg,
										void *data);

unsigned int						process_mode(
										const t_arg_parser_choice *const choice,
										const char *const arg,
										void *data);

unsigned int						process_stdin(
										const t_arg_parser_choice *const choice,
										const char *const arg,
										void *data);

unsigned int						process_file(
										const t_arg_parser_choice *const choice,
										const char *const arg,
										void *data);

t_modes								modes(void);

t_hash								md5(t_digest_block_getter *getter);

t_hash								sha256(t_digest_block_getter *getter);

#endif
