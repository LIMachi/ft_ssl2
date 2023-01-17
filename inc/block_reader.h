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

typedef size_t (*const					t_read)(void *obj, char *buff,
	size_t size);

struct									s_digest_block_reader{
	int				is_finished;
	int				is_big_endian;
	int				append_size_bytes;
	void			*obj;
	size_t			block_size;
	t_read			read;
	uint64_t		size;
};

struct									s_str_reader {
	size_t		head;
	const char	*str;
};

size_t									fd(void *obj, char *buff, size_t size);
size_t									str(void *obj, char *buff, size_t size);
int										read_block(
											t_digest_block_reader *reader,
											char *buff);

#endif
