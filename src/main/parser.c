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

inline uint64_t	flag(const char c)
{
	return (((uint64_t)1) << (c - 'a'));
}

int	process_flag(const t_choice *self, t_parse_result *out, const char *arg,
	void *state)
{
	((t_parser_state *)state)->flags |= ((uint64_t)1) << (self->alias - 'a');
	return (0);
}

int	process_string(const t_choice *self, t_parse_result *out, const char *arg,
	void *state)
{
	t_parser_state	*ps;

	if (arg == NULL)
	{
		out->err = INVALID_STRING;
		return (1);
	}
	ps = (t_parser_state *)state;
	ps->inputs[ps->input_count++] = (t_parser_input){INPUT_STRING, arg};
	out->argi++;
	return (0);
}

int	process_mode(const t_choice *self, t_parse_result *out, const char *arg,
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
	out->err = INVALID_MODE;
	return (1);
}

void	get_remainder_files(t_parser_state *state, int argc, t_argvp argv)
{
	int	i;

	i = 0;
	while (i < argc)
		state->inputs[state->input_count++] = (t_parser_input){
			INPUT_FILE, argv[i++]};
}
