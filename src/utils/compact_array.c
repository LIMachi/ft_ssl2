/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compact_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* truths:
* ((size_t)array[0] - (size_t)array) / sizeof(void*) == x
* (size_t)array[n + 1] - (size_t)array[n] == y
* free(array) -> no leak
*/

#include <malloc.h>

void	*allocate_compact_2d_array(size_t x, size_t y)
{
	size_t	i;
	void	*out;

	if (x == 0 || y == 0)
		return (NULL);
	out = malloc(sizeof(void *) * x + x * y);
	i = -1;
	while (++i < x)
		((void **) out)[i] = out + x * sizeof(void *) + i * y;
	return (out);
}
