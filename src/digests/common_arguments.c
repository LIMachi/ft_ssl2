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

#include "../argument_parser/argument_parser.h"
#include "../main/ft_ssl.h"

t_node	*digest_arguments(void)
{
	static t_node	out = (t_node){0, NULL, 0, NULL};
	static t_choice	choices[4] = {
	{'s', NULL, process_string, &out},
	{'p', NULL, process_flag, &out},
	{'q', NULL, process_flag, &out},
	{'r', NULL, process_flag, &out}};

	if (out.choice_count == 0)
	{
		out.choices = choices;
		out.choice_count = 4;
	}
	return (&out);
}
