/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print_utils.h"

void	write_maj(int fd, const char *str)
{
	size_t	l;

	l = -1;
	while (str[++l] != '\0')
	{
		if (str[l] >= 'a' && str[l] <= 'z')
			write(fd, &"ABCDEFGHIJKLMNOPQRSTUVWXYZ"[str[l] - 'a'], 1);
		else
			write(fd, &str[l], 1);
	}
}

void	write_hash(int fd, t_hash hash)
{
	size_t	i;

	i = 0;
	while (i < hash.hash_size / 8)
	{
		write(fd, &"0123456789abcdef"[hash.hash.h512.b[i] >> 4], 1);
		write(fd, &"0123456789abcdef"[hash.hash.h512.b[i] & 0xF], 1);
		++i;
	}
}

int	proto_printf(const char *pattern, const void **inputs)
{
	size_t	s;
	size_t	l;

	s = 0;
	while (pattern[s] != '\0')
	{
		if (pattern[s] == 's' || pattern[s] == 'l')
		{
			l = 0;
			while (((char *)inputs[s])[l] != '\0' && (pattern[s] == 's'
				|| ((char *)inputs[s])[l] != '\n'))
				++l;
			write(1, inputs[s], l);
		}
		if (pattern[s] == 'm')
			write_maj(1, inputs[s]);
		if (pattern[s] == 'h')
			write_hash(1, *((t_hash *)inputs[s]));
		if (pattern[s] == 'c' && inputs[s])
			++s;
		++s;
	}
	return (0);
}
