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

unsigned int	process_io(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	t_parser_state				*ps;
	int							*ptr;

	ps = (t_parser_state *)state;
	ptr = (int *)ps->extra;
	if (self->alias == 'i')
		++ptr;
	if (*ptr != 0)
		close(*ptr);
	if (self->alias == 'i')
		*ptr = open(arg, O_RDONLY);
	else
		*ptr = open(arg, O_WRONLY | O_CREAT | O_TRUNC);
	return (0);
}
