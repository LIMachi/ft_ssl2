/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_reader.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_READER_H
# define BLOCK_READER_H

# include <stdint.h>
# include <stddef.h>

typedef struct s_digest_block_reader	t_digest_block_reader;
typedef struct s_str_reader				t_str_reader;

typedef size_t (*const					t_read)(void *obj, uint8_t *buff,
	size_t size);

/**
* md5 digest block reader:
* {0, 0, 8, <OBJ>, 64, 4, <READ>, 0}
* sha256 digest block reader:
* {0, 1, 8, <OBJ>, 64, 4, <READ>, 0}
*/

struct									s_digest_block_reader{
	int			finished;
	int			big_endian;
	size_t		append_size_bytes;
	void		*obj;
	size_t		block_size;
	size_t		word_size;
	t_read		read;
	size_t		size;
};

struct									s_str_reader {
	size_t		head;
	const char	*str;
};

size_t									fd_read(void *obj, uint8_t *buff,
											size_t size);
size_t									str_read(void *obj, uint8_t *buff,
											size_t size);
int										read_block(
											t_digest_block_reader *reader,
											void *buff);

#endif
