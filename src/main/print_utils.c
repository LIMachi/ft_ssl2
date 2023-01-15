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

void	write_all_first_line(int fd, const char *str)
{
	size_t	l;

	l = 0;
	while (str[l] != '\0' && str[l] != '\n' && str[l] != '\r')
		++l;
	write(fd, str, l);
}

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

int	print_file_error(t_parser_state *state, size_t i)
{
	write(1, "ft_ssl: ", 8);
	write_all_first_line(1, modes().modes[state->mode].name);
	write(1, ": ", 2);
	write_all_first_line(1, state->inputs[i].arg);
	write(1, ": Cannot open file\n", 19);
	return (CANT_READ_FILE);
}
