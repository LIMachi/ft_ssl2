/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_arguments.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_ssl.h"
#include "print_utils.h"

unsigned int	process_string(const t_arg_parser_choice *const self,
	const char *const a, void *state)
{
	t_parser_state	*ps;
	t_bg_reader		reader;
	t_hash			hash;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	if (a == NULL)
		return (proto_printf(1, "sss", (t_va){"ft_ssl: ",
				ps->mode.name, ": missing string after -s\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf(1, "msls", (t_va){ps->mode.name, " (\"", a, "\") = "});
	reader = str_reader(a, -1);
	hash = ((t_hasher) ps->mode.run)(&reader);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf(1, "sl", (t_va){" \"", a, "\""});
	write(1, "\n", 1);
	return (0);
}

unsigned int	process_stdin(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state	*ps;
	t_bg_reader		reader;
	t_hash			hash;
	void			*quiet;
	void			*np;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	quiet = (void *)(ps->flags & (1 << ('q' - 'a')));
	np = (void *)(size_t)(arg == 0 || (arg[0] != 'p' && arg[0] != 's'));
	reader = fd_reader(0, 1 - 2 * (int)(size_t)np);
	if (np)
		hash = ((t_hasher) ps->mode.run)(&reader);
	if (!quiet)
		proto_printf(1, "scs", (t_va){"(", np, "\""});
	if (!np)
		hash = ((t_hasher) ps->mode.run)(&reader);
	if (!quiet)
		proto_printf(1, "cscss", (t_va){(void *)(size_t) !np, "stdin",
			np, "\"", ")= "});
	else if (!np)
		write(1, "\n", 1);
	return (proto_printf(1, "hs", (t_va){&hash, "\n"}));
}

unsigned int	process_file(const t_arg_parser_choice *const self,
	const char *const a, void *state)
{
	t_parser_state	*ps;
	t_bg_reader		reader;
	t_hash			hash;
	int				fd;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	fd = -1;
	if (a != 0)
		fd = open(a, O_RDONLY);
	if (fd < 0)
		return (FT_SSL_CANT_READ_FILE + proto_printf(1, "sssss",
				(t_va){"ft_ssl: ", ps->mode.name, ": ", a,
				": No such file or directory\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf(1, "msls", (t_va){ps->mode.name, " (", a, ") = "});
	reader = fd_reader(fd, -1);
	hash = ((t_hasher) ps->mode.run)(&reader);
	close(fd);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf(1, "sl", (t_va){" ", a});
	write(1, "\n", 1);
	return (0);
}

int	digest_cleanup(void *state, int ret, const int argc, t_csa argv)
{
	(void)ret;
	(void)argc;
	(void)argv;
	if (((t_parser_state *)state)->processed == 0 && ret >= 0)
		process_stdin(NULL, NULL, state);
	return (0);
}

t_arg_parser_node	*digest_arguments(void)
{
	static t_arg_parser_node	digest_files = (t_arg_parser_node){0, NULL, 0,
		NULL};
	static t_arg_parser_choice	dfc[1] = {{3, '\0', NULL, 0, process_file,
		&digest_files, NULL}};
	static t_arg_parser_node	out = (t_arg_parser_node){0, &digest_files, 0,
		NULL};
	static t_arg_parser_choice	outc[4] = {
	{2, 's', "--string", 1, process_string, &out, NULL},
	{1, 'p', "--stdin-passthrough", 0, process_stdin, &out, NULL},
	{0, 'q', "--quiet", 0, process_flag, &out, NULL},
	{0, 'r', "--reverse-print", 0, process_flag, &out, NULL}};

	if (out.count == 0)
	{
		digest_files.count = 1;
		digest_files.choices = dfc;
		out.count = 4;
		out.choices = outc;
	}
	return (&out);
}
