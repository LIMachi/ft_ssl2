/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_ssl.h"
#include "print_utils.h"
#include "endianese.h"

t_arg_parser_node	*base64_arguments(void)
{
	static t_arg_parser_node	out = (t_arg_parser_node){0, NULL, 6, NULL};
	static t_arg_parser_choice	outc[6] = {
	{0, 'i', "--input", 1, process_io, &out, NULL},
	{0, 'o', "--output", 1, process_io, &out, NULL},
	{0, 'e', "--encode", 0, process_flag, &out, NULL},
	{0, 'd', "--decode", 0, process_flag, &out, NULL},
	{0, 'n', "--insert-newlines", 0, process_flag, &out, NULL},
	{0, 'u', "--url-compat", 0, process_flag, &out, NULL}};

	if (out.choices == NULL)
		out.choices = outc;
	return (&out);
}

void	base64_encode(int in, int out, const char base[65], uint64_t new_line)
{
	uint32_t	buff_in;
	char		buff_out[4];
	ssize_t		r;
	ssize_t		i;
	size_t		acc;

	acc = 0;
	while (1)
	{
		buff_in = 0;
		r = read(in, &buff_in, 3);
		if (r <= 0)
			break ;
		if (new_line && ++acc % 16 == 1 && acc != 1)
			write(out, "\n", 1);
		if (little_endian())
			buff_in = swap_u32(buff_in) >> 8;
		i = -1;
		while (++i < 4)
			if (r - i < 0)
				buff_out[i] = base[64];
		else
			buff_out[i] = base[((buff_in >> ((3 - i) * 6)) & 63)];
		write(out, buff_out, 4);
	}
}

void	write_big_endian24(int fd, uint32_t v, size_t len)
{
	if (little_endian())
		v = swap_u32(v) >> 8;
	write(fd, &v, len);
}

void	base64_decode(int in, int out, const char base[65])
{
	char		buff_in;
	uint32_t	buff_out;
	size_t		acc;
	uint32_t	found;

	acc = 0;
	found = 0;
	buff_out = 0;
	while (read(in, &buff_in, 1) == 1 && found != 64)
	{
		found = 0;
		while (base[found] != buff_in && found < 65)
			++found;
		if (found >= 64)
			continue ;
		buff_out = (buff_out << 6) | found;
		if (++acc == 4)
		{
			write_big_endian24(out, buff_out, 3);
			acc = 0;
			buff_out = 0;
		}
	}
	write_big_endian24(out, buff_out << 6 * (4 - acc), acc - 1);
}

int	base64_cleanup(void *state, int ret, const int argc, t_csa argv)
{
	static char					base[65]
		= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	int							in;
	int							out;
	uint64_t					nl;

	(void)(ret + argc + argv);
	nl = ((t_parser_state *)state)->flags & (((uint64_t)1) << ('n' - 'a'));
	if (((t_parser_state *)state)->flags & (((uint64_t)1) << ('u' - 'a')))
		*(uint16_t*)&base[62] = *(uint16_t*)"-_";
	if (*(int *)((t_parser_state *)state)->extra != 0)
		out = *(int *)((t_parser_state *)state)->extra;
	else
		out = 1;
	in = *(((int *)((t_parser_state *)state)->extra) + 1);
	if ((((t_parser_state *)state)->flags & (((uint64_t)1) << ('d' - 'a')))
		&& !(((t_parser_state *)state)->flags & (((uint64_t)1) << ('e' - 'a'))))
		base64_decode(in, out, base);
	else
		base64_encode(in, out, base, nl);
	if (in > 2)
		close(in);
	if (out > 2)
		close(out);
	return (0);
}
