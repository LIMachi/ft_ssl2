/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endianese.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

int	little_endian(void)
{
	static const uint16_t	test = 1;

	return (*((uint8_t *) &test));
}

uint32_t	swap_u32(uint32_t v)
{
	return ((v >> 24) | ((v >> 8) & 0xFF00)
		| ((v << 8) & 0xFF0000) | (v << 24));
}

uint16_t	swap_u16(uint16_t v)
{
	return ((v >> 8) | (v << 8));
}

uint64_t	swap_u64(uint64_t v)
{
	return ((v >> 56) | ((v >> 40) & 0xFF00) | ((v >> 24) & 0xFF0000)
		| ((v >> 8) & 0xFF000000) | ((v << 8) & 0xFF00000000)
		| ((v << 24) & 0xFF0000000000) | ((v << 40) & 0xFF000000000000)
		| (v << 56));
}

void	*swap_array(void *array, size_t length, size_t block)
{
	size_t	i;

	i = -1;
	if (block == 2)
		while (++i < length)
			((uint16_t *)array)[i] = swap_u16(((uint16_t *)array)[i]);
	if (block == 4)
		while (++i < length)
			((uint32_t *)array)[i] = swap_u32(((uint32_t *)array)[i]);
	if (block == 8)
		while (++i < length)
			((uint64_t *)array)[i] = swap_u64(((uint64_t *)array)[i]);
	return (array);
}
