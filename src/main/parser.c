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

#include "ft_ssl.h"
#include "block_getter.h"

int	process_flag(const t_choice *self, t_consume_params params, int *error,
	void *state)
{
	((t_parser_state *)state)->flags |= ((uint64_t)1) << (self->alias - 'a');
	return (0);
}

int	process_string(const t_choice *self, t_consume_params params, int *error,
	void *state)
{
	t_parser_state				*ps;
	size_t						l;
	t_digest_block_descriptor	des;
	t_digest_block_getter		reader;

	if (params.next_arg == NULL)
	{
		*error = FT_SSL_INVALID_STRING;
		return (1);
	}
	ps = (t_parser_state *)state;
	if (ps->mode == 0)
		des = descriptor(64, 4, 8, 0);
	else if (ps->mode == 1)
		des = descriptor(64, 4, 8, 1);
	reader = str_getter(params.next_arg);
	md5(params.next_arg, )
//	l = 0;
//	while (params.next_arg[l] != '\0')
//		++l;
//	ps->inpts[ps->cinputs++] = (t_input){INPUT_STRING, params.next_arg, l,
//		params.next_arg};
	*params.jump_arg = 1;
	return (0);
}

int	process_mode(const t_choice *self, t_consume_params params, int *error,
	void *state)
{
	size_t			i;
	t_parser_state	*ps;
	t_modes			ml;

	i = 0;
	ml = modes();
	ps = (t_parser_state *)state;
	while (i < ml.mode_count)
	{
		if (str_match(ml.modes[i].name, self->name))
		{
			ps->mode = i;
			return (0);
		}
	}
	*error = FT_SSL_INVALID_MODE;
	return (1);
}

int	process_stdin(const t_choice *self, t_consume_params params, int *error,
	void *state)
{

}

int	process_file(const t_choice *self, t_consume_params params, int *error,
	void *state)
{

}
