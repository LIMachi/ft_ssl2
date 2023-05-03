/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_getter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "block_getter.h"
#include "endianese.h"

/**
* Put the total read size of the object in the buffer, taking into account the
* expected size in bytes of the total and the endianese.
*/
static inline void	put_int(uint64_t var, uint8_t *buff, size_t sizeof_var,
	int big_endian)
{
	char			*ptr;
	size_t			i;

	if (sizeof_var == 0)
		return ;
	if (!little_endian())
		ptr = &((char *)&var)[8 - sizeof_var];
	else
		ptr = (char *)&var;
	i = -1;
	if (big_endian != little_endian())
		while (++i < sizeof_var)
			buff[i] = ptr[i];
	else
		while (++i < sizeof_var)
			buff[i] = ptr[sizeof_var - i - 1];
}

size_t	fd_read(union u_digest_target *target, uint8_t *buff, size_t size,
	int *print)
{
	ssize_t		r;
	ssize_t		i;

	r = read(target->fd, buff, size);
	if (r < 0)
		return (0);
	if (*print >= 0)
	{
		i = 0;
		while (i < r && buff[i] != '\n' && buff[i] != '\r')
			++i;
		write(*print, buff, (size_t)i);
		if (buff[i] == '\n' || buff[i] == '\r')
			*print = -1;
	}
	return ((size_t)r);
}

size_t	str_read(union u_digest_target *target, uint8_t *buff, size_t size,
	int *print)
{
	size_t			r;
	size_t			i;

	r = -1;
	while (++r < size && target->str.ptr[target->str.head] != '\0')
		buff[r] = target->str.ptr[target->str.head++];
	if (*print >= 0)
	{
		i = 0;
		while (i < r && buff[i] != '\n' && buff[i] != '\r')
			++i;
		write(*print, buff, (size_t)i);
		if (buff[i] == '\n' || buff[i] == '\r')
			*print = -1;
	}
	return (r);
}

int	read_block(const t_block_descriptor *desc,
	t_block_getter *reader, void *buff)
{
	size_t	r;

	if (reader->finished)
		return (0);
	r = reader->read(&reader->target, buff, desc->block_size,
			&reader->print);
	if (r > 0)
		reader->size += r;
	if (r == desc->block_size)
		return (1);
	if (r > 0)
		((uint8_t *)buff)[r++] = 0x80;
	if (r + desc->append_size_bytes < desc->block_size)
	{
		while (r + desc->append_size_bytes < desc->block_size)
			((uint8_t *)buff)[r++] = 0;
		put_int(reader->size * 8, &((uint8_t *)buff)[r],
			desc->append_size_bytes, desc->big_endian);
		reader->finished = 1;
	}
	else
		while (r < desc->block_size)
			((uint8_t *)buff)[r++] = 0;
	swap_array(buff, desc->block_size / desc->word_size, desc->word_size);
	return (1);
}
