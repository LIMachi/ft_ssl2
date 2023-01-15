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
#include "../main/ft_ssl.h"
#include "../main/print_utils.h"

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

void	print_stdin(t_parser_state *state, char *hash, size_t hash_length)
{
	int			nq;

	nq = (state->flags & flag('q')) == 0;
	write(1, "(", nq);
	if (state->flags & flag('p'))
	{
		write(1, "\"", nq);
		write_all_first_line(1, state->inputs[state->input_count - 1].data);
		write(1, "\"", nq);
		write(1, "\n", !nq);
	}
	else if (nq)
		write(1, "stdin", 5);
	write(1, ")= ", 3 * nq);
	write_hash(hash, hash_length);
	write(1, "\n", 1);
}

void	print_file_or_string(t_parser_state *state, char *hash,
	size_t index, size_t hash_length)
{
	int	quiet;
	int	reversed;
	int	string;

	quiet = (state->flags & flag('q')) != 0;
	reversed = (state->flags & flag('r')) != 0;
	string = state->inputs[index].type == INPUT_STRING;
	if (!quiet && !reversed)
	{
		write_maj(1, modes().modes[state->mode].name);
		write(1, " (\"", 2 + string);
		write_all_first_line(1, state->inputs[index].arg);
		write(1, &"\") = "[1 - string], 4 + string);
	}
	write_hash(hash, hash_length);
	if (!quiet && reversed)
	{
		write(1, " \"", 1 + string);
		write_all_first_line(1, state->inputs[index].arg);
		write(1, "\"", string);
	}
	write(1, "\n", 1);
}

void	free_state_and_hashes(t_parser_state *state, char **hashes)
{
	size_t	i;

	i = -1;
	if (state != NULL)
		while (++i < state->input_count)
			if (state->inputs[i].type == INPUT_FILE
				|| state->inputs[i].type == STDIN)
				free((void *)state->inputs[i].data);
	if (hashes != NULL)
		free(hashes);
}

int	print_hashes(int read_stdin, t_parser_state *state, char **hashes, size_t hash_length)
{
	size_t	i;

	if (read_stdin)
		print_stdin(state, hashes[state->input_count - 1], hash_length);
	i = -1;
	while (++i < state->input_count - read_stdin)
		print_file_or_string(state, hashes[i], i, hash_length);
	free_state_and_hashes(state, hashes);
	return (0);
}
