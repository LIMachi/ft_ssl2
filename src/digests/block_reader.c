/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "block_reader.h"

/**
* Put the total read size of the object in the buffer, taking into account the
* expected size in bytes of the total and the endianese.
*/
static inline void	put_int(uint64_t var, uint8_t *buff, size_t sizeof_var,
	int big_endian)
{
	const uint16_t	endian_test = 1;
	int				little_endian;
	char			*ptr;
	int				i;

	little_endian = *(uint8_t *)&endian_test;
	if (!little_endian)
		ptr = &((char *)&var)[8 - sizeof_var];
	else
		ptr = (char *)&var;
	i = -1;
	if (big_endian != little_endian)
		while (++i < sizeof_var)
			buff[i] = ptr[i];
	else
		while (++i < sizeof_var)
			buff[i] = ptr[sizeof_var - i - 1];
}

/**
* If needed (machine endian != target endian), swap the endian of the buffer,
* word per word, except the final int (already correct endian).
* The reason we do not swap the final in is because some digest use a final int
* which size is not equal to a word (ex sha256 will use a uint64_t size but use
* uint32_t words).
*/
static inline void	swap_endian(uint8_t *buff, t_digest_block_reader *reader)
{
	const uint16_t	little_endian_test = 1;
	size_t			i;
	size_t			j;
	uint8_t			t;
	size_t			l;

	if (*(uint8_t *)&little_endian_test != reader->big_endian)
		return;
	l = reader->block_size - reader->finished * reader->append_size_bytes;
	i = 0;
	while (i < l)
	{
		j = 0;
		while (j < reader->word_size / 2)
		{
			t = buff[i + j];
			buff[i + j] = buff[i + reader->word_size - j - 1];
			buff[i + reader->word_size - j++ - 1] = t;
		}
		i += reader->word_size;
	}
}

size_t	fd_read(void *obj, uint8_t *buff, size_t size)
{
	ssize_t			r;

	r = read(*(const int *)obj, buff, size);
	if (r < 0)
		return (0);
	return (r);
}

size_t	str_read(void *obj, uint8_t *buff, size_t size)
{
	size_t			r;
	t_str_reader	*o;

	o = (t_str_reader *)obj;
	r = -1;
	while (++r < size && o->str[o->head] != '\0')
		buff[r] = o->str[o->head++];
	return (r);
}

/**
* Populate the given buff with data from the reader,
* Padded with 1 bit + 0 + total size of read object.
*/
int	read_block(t_digest_block_reader *reader, void *buff)
{
	size_t	r;

	if (reader->finished)
		return (0);
	r = reader->read(reader->obj, buff, reader->block_size);
	if (r > 0)
		reader->size += r;
	if (r == reader->block_size)
		return (1);
	if (r > 0)
		((uint8_t *)buff)[r++] = 0x80;
	if (r + reader->append_size_bytes < reader->block_size)
	{
		while (r + reader->append_size_bytes < reader->block_size)
			((uint8_t *)buff)[r++] = 0;
		put_int(reader->size * 8, &((uint8_t *)buff)[r],
			reader->append_size_bytes, reader->big_endian);
		reader->finished = 1;
	}
	else
		while (r < reader->block_size)
			((uint8_t *)buff)[r++] = 0;
	swap_endian(buff, reader);
	return (1);
}
