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

#include "alloc_less_argv_parser.h"
#include "ft_ssl.h"

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
