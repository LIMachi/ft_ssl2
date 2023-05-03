/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"
#include "alloc_less_argv_parser.h"

uint64_t	des_expansion_and_xor(uint32_t right, size_t it,
	const uint64_t sub_keys[16], int decrypt)
{
	const size_t	shifts[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9,
		10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
		22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
	size_t			i;
	uint64_t		out;

	out = 0;
	i = -1;
	while (++i < 48)
		out = (out << 1) | ((uint64_t)(right >> (32 - shifts[i])) & 1);
	if (decrypt)
		return (out ^ sub_keys[15 - it]);
	return (out ^ sub_keys[it]);
}

uint32_t	des_box_round(uint64_t in, uint32_t *inner_acc)
{
	const size_t	shifts[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26,
		5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4,
		25};
	size_t			i;
	uint8_t			row;
	uint8_t			column;
	uint32_t		out;

	i = -1;
	while (++i < 8)
	{
		row = (uint8_t)((in & (0x840000000000 >> (6 * i))) >> (42 - 6 * i));
		row = (row >> 4) | (row & 1);
		column = (uint8_t)((in & (0x780000000000 >> (6 * i))) >> (43 - 6 * i));
		*inner_acc <<= 4;
		*inner_acc |= des_box(i, row, column);
	}
	out = 0;
	i = -1;
	while (++i < 32)
		out = (out << 1) | ((*inner_acc >> (32 - shifts[i])) & 1);
	return (out);
}

uint64_t	des(uint64_t in, uint64_t key, int decrypt)
{
	uint32_t	sides[3];
	uint32_t	ks[2];
	uint64_t	sub_keys[16];
	uint32_t	box_acc;
	size_t		i;

	box_acc = 0;
	in = des_initial_permutation(in, 0);
	sides[0] = (uint32_t)(in >> 32);
	sides[1] = (uint32_t)in;
	des_initial_key_permutation(key, ks);
	des_initialize_sub_keys(ks, sub_keys);
	i = -1;
	while (++i < 16)
	{
		sides[2] = sides[1];
		sides[1] = sides[0] ^ des_box_round(
				des_expansion_and_xor(sides[1], i, sub_keys, decrypt),
				&box_acc);
		sides[0] = sides[2];
	}
	return (des_initial_permutation(
			(((uint64_t)sides[1]) << 32) | (uint64_t)sides[0], 1));
}

t_arg_parser_node	*des_arguments(void)
{

}

int	des_cleanup(void *state, int ret, const int argc, t_csa argv)
{

}
