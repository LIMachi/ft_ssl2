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

static inline void	put_endianesed_int(uint64_t var, char *buff, int sizeof_var,
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

size_t	fd(void *obj, char *buff, size_t size)
{
	ssize_t	r;

	r = read(*(int *)obj, buff, size);
	if (r < 0)
		return (0);
	return (r);
}

size_t	str(void *obj, char *buff, size_t size)
{
	size_t			i;
	t_str_reader	*o;

	o = (t_str_reader *)obj;
	i = -1;
	while (++i < size && o->str[i + o->head] != '\0')
		buff[i] = o->str[i + o->head++];
	return (i);
}

int	read_block(t_digest_block_reader *reader, char *buff)
{
	size_t	r;

	if (reader->is_finished)
		return (0);
	r = reader->read(reader->obj, buff, reader->block_size);
	if (r > 0)
		reader->size += r;
	if (r == reader->size)
		return (1);
	if (r > 0)
		buff[r++] = 0x80;
	if (r + reader->append_size_bytes < reader->block_size)
	{
		while (r + reader->append_size_bytes < reader->block_size)
			buff[r++] = 0;
		put_endianesed_int(reader->size, &buff[r], reader->append_size_bytes,
			reader->is_big_endian);
	}
	else
		while (r < reader->block_size)
			buff[r++] = 0;
	return (1);
}
