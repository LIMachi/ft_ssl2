/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include "ft_ssl.h"
#include "print_utils.h"

void	write_hash(const char *hash, size_t hash_length)
{
	size_t	i;

	i = -1;
	while (++i < hash_length)
	{
		write(1, &"0123456789abcdef"[(hash[i] & 0xF0) >> 4], 1);
		write(1, &"0123456789abcdef"[(hash[i] & 0xF)], 1);
	}
}

void	print_stdin(const char *in, char *hash, size_t hash_size, uint64_t flags)
{
	int			nq;

	nq = (flags & flag('q')) == 0;
	write(1, "(", nq);
	if (flags & flag('p'))
	{
		write(1, "\"", nq);
		write_all_first_line(1, in);
		write(1, "\"", nq);
		write(1, "\n", !nq);
	}
	else if (nq)
		write(1, "stdin", 5);
	write(1, ")= ", 3 * nq);
	write_hash(hash, hash_size);
	write(1, "\n", 1);
}

/*
void	print_stdin(t_parser_state *state, char *hash, size_t hash_length)
{
	int			nq;

	nq = (state->flags & flag('q')) == 0;
	write(1, "(", nq);
	if (state->flags & flag('p'))
	{
		write(1, "\"", nq);
		write_all_first_line(1, state->inpts[state->cinputs - 1].data);
		write(1, "\"", nq);
		write(1, "\n", !nq);
	}
	else if (nq)
		write(1, "stdin", 5);
	write(1, ")= ", 3 * nq);
	write_hash(hash, hash_length);
	write(1, "\n", 1);
}
 */

void	print_file_or_string(t_parser_state *state, char *hash,
	size_t index, size_t hash_length)
{
	int	quiet;
	int	reversed;
	int	string;

	quiet = (state->flags & flag('q')) != 0;
	reversed = (state->flags & flag('r')) != 0;
	string = state->inpts[index].type == INPUT_STRING;
	if (!quiet && !reversed)
	{
		write_maj(1, modes().modes[state->mode].name);
		write(1, " (\"", 2 + string);
		write_all_first_line(1, state->inpts[index].arg);
		write(1, &"\") = "[1 - string], 4 + string);
	}
	write_hash(hash, hash_length);
	if (!quiet && reversed)
	{
		write(1, " \"", 1 + string);
		write_all_first_line(1, state->inpts[index].arg);
		write(1, "\"", string);
	}
	write(1, "\n", 1);
}

void	free_state_and_hashes(t_parser_state *state, char **hashes)
{
	size_t	i;

	i = -1;
	if (state != NULL)
		while (++i < state->cinputs)
			if (state->inpts[i].type == INPUT_FILE
				|| state->inpts[i].type == STDIN)
				free((void *)state->inpts[i].data);
	if (hashes != NULL)
		free(hashes);
}

int	print_hashes(int read_stdin, t_parser_state *state, char **hashes,
	size_t hash_length)
{
	size_t	i;

//	if (read_stdin)
//		print_stdin(state, hashes[state->cinputs - 1], hash_length);
	i = -1;
	while (++i < state->cinputs - read_stdin)
		print_file_or_string(state, hashes[i], i, hash_length);
	free_state_and_hashes(state, hashes);
	return (0);
}
