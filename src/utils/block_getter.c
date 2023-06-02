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

size_t	str_read_func(t_bg_reader *self, uint8_t *buff,
	const t_bg_descriptor *desc)
{
	size_t			r;
	size_t			i;

	if (self->finished)
		return (0);
	r = -1;
	while (++r < desc->block_size * desc->word_size
		&& self->str_ptr[self->head_or_fd] != '\0')
		buff[r] = self->str_ptr[self->head_or_fd++];
	i = r;
	while (i < desc->block_size * desc->word_size)
		buff[i++] = 0;
	if (self->forward >= 0)
	{
		i = 0;
		while (i < r && buff[i] != '\n' && buff[i] != '\r')
			++i;
		write(self->forward, buff, i);
		if (buff[i] == '\n' || buff[i] == '\r')
			self->forward = -1;
	}
	return (r);
}

t_bg_reader	str_reader(const char *str, int forward)
{
	return ((t_bg_reader){0, 0, str, forward, str_read_func, 0, 0});
}

size_t	fd_read_func(t_bg_reader *self, uint8_t *buff,
	const t_bg_descriptor *desc)
{
	ssize_t			r;
	size_t			i;

	if (self->finished)
		return (0);
	r = read(self->head_or_fd, buff, desc->block_size * desc->word_size);
	if (r < 0)
		r = 0;
	i = (size_t)r;
	while (i < desc->block_size * desc->word_size)
		buff[i++] = 0;
	if (self->forward >= 0)
	{
		i = 0;
		while (i < (size_t)r && buff[i] != '\n' && buff[i] != '\r')
			++i;
		write(self->forward, buff, (size_t)i);
		if (buff[i] == '\n' || buff[i] == '\r')
			self->forward = -1;
	}
	return ((size_t)r);
}

t_bg_reader	fd_reader(int fd, int forward)
{
	return ((t_bg_reader){0, fd, 0, forward, fd_read_func, 0, 0});
}

int	read_block(const t_bg_descriptor *desc, t_bg_reader *reader, void *buf)
{
	size_t	r;

	if (reader->finished)
		return (0);
	r = reader->reader(reader, buf, desc);
	if (r > 0)
		reader->size += r;
	if (r != desc->word_size * desc->block_size)
	{
		if (desc->func)
			reader->finished = !desc->func(buf, r, reader->size, reader->rep++);
		else
			reader->finished = 1;
	}
	if (desc->big_endian == little_endian())
		swap_array(buf, desc->block_size, desc->word_size);
	return (1);
}
