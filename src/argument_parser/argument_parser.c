/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argument_parser.h"

int	parse_try_choice(t_parse_result *const out, t_parser parser, size_t ci,
	const t_node **const node)
{
	const char	*arg;

	if (out->argi + 1 < parser.argc)
		arg = parser.argv[out->argi + 1];
	else
		arg = NULL;
	if ((*node)->choices[ci].callback != NULL
		&& (*node)->choices[ci].callback(
			&(*node)->choices[ci], out, arg, parser.state))
		return (1);
	if ((*node)->choices[ci].next != NULL)
		*node = (*node)->choices[ci].next;
	return (0);
}

t_parse_result	parse(const t_parser parser, const t_node *node)
{
	t_parse_result	out;
	size_t			ci;

	out = (t_parse_result){0, 1};
	while (out.argi < parser.argc && node != NULL)
	{
		ci = 0;
		while (ci < node->choice_count && !((parser.argv[out.argi][0] == '-'
				&& parser.argv[out.argi][1] == node->choices[ci].alias)
				|| str_match(parser.argv[out.argi], node->choices[ci].name)))
			++ci;
		if (ci >= node->choice_count)
		{
			out.err = node->missing_error;
			if (out.err != 0)
				return (out);
			node = node->next;
		}
		else if (parse_try_choice(&out, parser, ci, &node))
			return (out);
		if (node != NULL)
			++out.argi;
	}
	return (out);
}

t_parser	create_parser(const int argc, t_argvp argv, void *state)
{
	return ((t_parser){argc, argv, state});
}
