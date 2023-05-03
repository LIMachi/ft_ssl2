/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_getter.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_GETTER_H
# define BLOCK_GETTER_H

# include <stdint.h>
# include <stddef.h>

typedef struct s_block_getter		t_block_getter;
typedef struct s_block_descriptor	t_block_descriptor;

struct								s_str_reader {
	size_t		head;
	const char	*ptr;
};

union								u_digest_target {
	int					fd;
	struct s_str_reader	str;
};

typedef size_t						(*t_read)(
		union u_digest_target *target, uint8_t *buff, size_t size, int *print);

/**
* md5 digest block reader:
* {0, 0, 8, <OBJ>, 64, 4, <READ>, 0}
* sha256 digest block reader:
* {0, 1, 8, <OBJ>, 64, 4, <READ>, 0}
*/

struct								s_block_descriptor {
	const int		big_endian;
	const size_t	append_size_bytes;
	const size_t	block_size;
	const size_t	word_size;
	uint8_t			first_pad;
	uint8_t			extra_pad;
};

struct								s_block_getter {
	int						finished;
	union u_digest_target	target;
	t_read					read;
	size_t					size;
	int						print;
};

size_t				fd_read(union u_digest_target *target,
						uint8_t *buff, size_t size, int *print);
size_t				str_read(union u_digest_target *target,
						uint8_t *buff,	size_t size, int *print);
int					read_block(const t_block_descriptor *desc,
						t_block_getter *reader, void *buff);

t_block_getter		fd_getter(int fd, int print);
t_block_getter		str_getter(const char *str, int print);

#endif
