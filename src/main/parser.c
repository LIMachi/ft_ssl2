/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "ft_ssl.h"
#include "block_getter.h"
#include "print_utils.h"

unsigned int	process_flag(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	((t_parser_state *)state)->flags |= ((uint64_t)1) << (self->alias - 'a');
	return (0);
}

unsigned int	process_string(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;

	ps = (t_parser_state *)state;
	++ps->processed;
	if (arg == NULL)
		return (proto_printf("sss", (const void *[3]){"ft_ssl: ",
				modes().modes[ps->mode].name, ": missing string after -s\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf("msls", (const void *[4]){modes().modes[ps->mode].name,
			" (\"", arg, "\") = "});
	reader = str_getter(arg, -1);
	hash = ((t_hasher) ps->internal)(&reader);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf("sl", (const void *[3]){" \"", arg, "\""});
	write(1, "\n", 1);
	return (0);
}

unsigned int	process_mode(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	size_t			i;
	t_parser_state	*ps;
	t_modes			ml;

	i = 0;
	ml = modes();
	ps = (t_parser_state *)state;
	while (i < ml.mode_count)
	{
		if (strcmp(ml.modes[i].name, self->label) == 0)
		{
			ps->mode = i;
			ps->internal = ml.modes[i].run;
			return (0);
		}
		++i;
	}
	return (FT_SSL_INVALID_MODE);
}

unsigned int	process_stdin(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;
	void						*quiet;
	void						*print;

	ps = (t_parser_state *)state;
	++ps->processed;
	quiet = (void *)(ps->flags & (1 << ('q' - 'a')));
	print = (void *)(size_t)(arg != 0 && arg[0] == 'p');
	if (!quiet)
		proto_printf("scs", (const void *[3]){"(", print, "\""});
	if (print)
		reader = fd_getter(0, 1);
	else
		reader = fd_getter(0, -1);
	hash = ((t_hasher) ps->internal)(&reader);
	if (!quiet)
		proto_printf("cscss", (const void *[]){print, "stdin",
			(void *)(size_t) !print, "\"", ")= "});
	else if (print)
		write(1, "\n", 1);
	return (proto_printf("hs", (const void *[2]){&hash, "\n"}));
}

unsigned int	process_file(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;
	int							fd;

	ps = (t_parser_state *)state;
	++ps->processed;
	fd = -1;
	if (arg != 0)
		fd = open(arg, O_RDONLY);
	if (fd < 0)
		return (proto_printf("sssss", (const void *[5]){"ft_ssl: ",
				modes().modes[ps->mode].name, ": ", arg,
				": No such file or directory\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf("msls", (const void *[4]){modes().modes[ps->mode].name,
			" (", arg, ") = "});
	reader = fd_getter(fd, -1);
	hash = ((t_hasher) ps->internal)(&reader);
	close(fd);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf("sl", (const void *[2]){" ", arg});
	write(1, "\n", 1);
	return (0);
}
