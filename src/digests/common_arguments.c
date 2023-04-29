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
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	if (a == NULL)
		return (proto_printf("sss", (const void *[3]){"ft_ssl: ",
				ps->mode.name, ": missing string after -s\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf("msls", (const void *[4]){ps->mode.name, " (\"", a,
			"\") = "});
	reader = str_getter(a, -1);
	hash = ((t_hasher) ps->mode.run)(&reader);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf("sl", (const void *[3]){" \"", a, "\""});
	write(1, "\n", 1);
	return (0);
}

unsigned int	process_stdin(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;
	void						*quiet;
	void						*np;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	quiet = (void *)(ps->flags & (1 << ('q' - 'a')));
	np = (void *)(size_t)(arg == 0 || arg[0] != 'p');
	reader = fd_getter(0, 1 - 2 * (int)(size_t)np);
	if (np)
		hash = ((t_hasher) ps->mode.run)(&reader);
	if (!quiet)
		proto_printf("scs", (const void *[3]){"(", np, "\""});
	if (!np)
		hash = ((t_hasher) ps->mode.run)(&reader);
	if (!quiet)
		proto_printf("cscss", (const void *[]){(void *)(size_t) !np, "stdin",
			np, "\"", ")= "});
	else if (!np)
		write(1, "\n", 1);
	return (proto_printf("hs", (const void *[2]){&hash, "\n"}));
}

unsigned int	process_file(const t_arg_parser_choice *const self,
	const char *const a, void *state)
{
	t_parser_state				*ps;
	t_digest_block_getter		reader;
	t_hash						hash;
	int							fd;

	(void)self;
	ps = (t_parser_state *)state;
	++ps->processed;
	fd = -1;
	if (a != 0)
		fd = open(a, O_RDONLY);
	if (fd < 0)
		return (proto_printf("sssss", (const void *[5]){"ft_ssl: ",
				ps->mode.name, ": ", a, ": No such file or directory\n"}));
	if (!(ps->flags & (1 << ('q' - 'a'))) && !(ps->flags & (1 << ('r' - 'a'))))
		proto_printf("msls", (const void *[4]){ps->mode.name, " (", a, ") = "});
	reader = fd_getter(fd, -1);
	hash = ((t_hasher) ps->mode.run)(&reader);
	close(fd);
	write_hash(1, hash);
	if (!(ps->flags & (1 << ('q' - 'a'))) && (ps->flags & (1 << ('r' - 'a'))))
		proto_printf("sl", (const void *[2]){" ", a});
	write(1, "\n", 1);
	return (0);
}

int	digest_cleanup(void *state, int ret, const int argc, t_csa argv)
{
	(void)ret;
	(void)argc;
	(void)argv;
	if (((t_parser_state *)state)->processed == 0)
		process_stdin(NULL, NULL, state);
	return (0);
}

t_arg_parser_node	*digest_arguments(void)
{
	static t_arg_parser_node	digest_files = (t_arg_parser_node){0, NULL, 0,
		NULL};
	static t_arg_parser_choice	dfc[1] = {{3, '\0', NULL, 0, process_file,
		&digest_files}};
	static t_arg_parser_node	out = (t_arg_parser_node){0, &digest_files, 0,
		NULL};
	static t_arg_parser_choice	outc[4] = {
	{2, 's', NULL, 1, process_string, &out},
	{1, 'p', NULL, 0, process_stdin, &out},
	{0, 'q', NULL, 0, process_flag, &out},
	{0, 'r', NULL, 0, process_flag, &out}};

	if (out.count == 0)
	{
		digest_files.count = 1;
		digest_files.choices = dfc;
		out.count = 4;
		out.choices = outc;
	}
	return (&out);
}
