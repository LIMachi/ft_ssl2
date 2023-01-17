/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>

char	*append(char *to, size_t size, const char *from, size_t add)
{
	char	*out;
	size_t	i;

	out = malloc(size + add);
	if (out == NULL)
	{
		free(to);
		return (NULL);
	}
	i = -1;
	if (to != NULL)
		while (++i < size)
			out[i] = to[i];
	i = -1;
	while (++i < add)
		out[size + i] = from[i];
	if (to != NULL)
		free(to);
	return (out);
}

char	*read_fd(int fd, size_t *out_size)
{
	char	buff[4096];
	char	*out;
	size_t	s;
	ssize_t	r;

	s = 0;
	out = NULL;
	while (1)
	{
		r = read(fd, buff, 4096);
		if (r <= 0)
			break ;
		out = append(out, s, buff, r);
		if (out == NULL)
			break ;
		s += r;
	}
	if (out_size != NULL)
		*out_size = r;
	if (r < 0 && out != NULL)
		free(out);
	if (r < 0)
		return (NULL);
	return (out);
}

ssize_t	file_size(const char *path)
{
	int		fd;
	ssize_t	size;
	ssize_t	ts;
	char	buff[4096];

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	size = 0;
	while (1)
	{
		ts = read(fd, buff, 4096);
		if (ts > 0)
			size += ts;
		else if (ts == 0)
			break ;
		else
			return (-1);
	}
	close(fd);
	return (size);
}

char	*read_file(const char *path, size_t *out_size)
{
	int		fd;
	char	*out;
	ssize_t	size;

	size = file_size(path);
	if (size < 0)
		return (NULL);
	out = malloc(sizeof(char) * size);
	if (out_size != NULL)
		*out_size = size;
	fd = open(path, O_RDONLY);
	read(fd, out, size);
	close(fd);
	return (out);
}
