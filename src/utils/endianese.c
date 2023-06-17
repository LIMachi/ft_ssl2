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

uint64_t	swap_u64(uint64_t v)
{
	return ((v >> 56) | ((v >> 40) & 0xFF00) | ((v >> 24) & 0xFF0000)
		| ((v >> 8) & 0xFF000000) | ((v << 8) & 0xFF00000000)
		| ((v << 24) & 0xFF0000000000) | ((v << 40) & 0xFF000000000000)
		| (v << 56));
}

__uint128_t	swap_u128(__uint128_t v)
{
	return (((v >> 120) & (((__uint128_t)0xFF) << 0))
		| ((v >> 104) & (((__uint128_t)0xFF) << 8))
		| ((v >> 88) & (((__uint128_t)0xFF) << 16))
		| ((v >> 72) & (((__uint128_t)0xFF) << 24))
		| ((v >> 56) & (((__uint128_t)0xFF) << 32))
		| ((v >> 40) & (((__uint128_t)0xFF) << 40))
		| ((v >> 24) & (((__uint128_t)0xFF) << 48))
		| ((v >> 8) & (((__uint128_t)0xFF) << 56))
		| ((v << 8) & (((__uint128_t)0xFF) << 64))
		| ((v << 24) & (((__uint128_t)0xFF) << 72))
		| ((v << 40) & (((__uint128_t)0xFF) << 80))
		| ((v << 56) & (((__uint128_t)0xFF) << 88))
		| ((v << 72) & (((__uint128_t)0xFF) << 96))
		| ((v << 88) & (((__uint128_t)0xFF) << 104))
		| ((v << 104) & (((__uint128_t)0xFF) << 112))
		| ((v << 120) & (((__uint128_t)0xFF) << 120)));
}

void	*swap_array(void *array, size_t length, size_t block)
{
	size_t	i;

	i = -1;
	if (block == 2)
		while (++i < length)
			((uint16_t *)array)[i] = ((((uint16_t *)array)[i] & 0xFF00) >> 8)
				| ((((uint16_t *)array)[i] & 0xFF) << 8);
	if (block == 4)
		while (++i < length)
			((uint32_t *)array)[i] = swap_u32(((uint32_t *)array)[i]);
	if (block == 8)
		while (++i < length)
			((uint64_t *)array)[i] = swap_u64(((uint64_t *)array)[i]);
	return (array);
}
