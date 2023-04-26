/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

/**
* md5sum test.txt ->                        35310a49e26265a58122d9c6e0823f49
* ./ft_ssl md5 test.txt -> MD5 (test.txt) = 35310a49e26265a58122d9c6e0823f49
* md5sum test2.txt ->                       098f6bcd4621d373cade4e832627b4f6
* ./ft_ssl md5 test2.txt ->                 098f6bcd4621d373cade4e832627b4f6
*/

void	rotate(union u_128hash *hash, uint32_t f, uint32_t w, uint32_t i)
{
	uint32_t		x;
	uint32_t		t;
	uint32_t		shift;
	const uint32_t	usine[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
		0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453,
		0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681,
		0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5,
		0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0,
		0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
	const uint32_t	ushift[16] = {7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6,
		10, 15, 21};

	shift = ushift[i & 3 | ((i & 0xF0) >> 2)];
	x = hash->w.a + f + usine[i] + w;
	t = hash->w.d;
	hash->w.d = hash->w.c;
	hash->w.c = hash->w.b;
	hash->w.b += (x << shift) | (x >> (32 - shift));
	hash->w.a = t;
}

void	iteration(union u_128hash *h, uint32_t w[16], uint32_t i)
{
	uint32_t	f;
	uint32_t	g;

	if (i < 16)
	{
		f = (h->w.b & h->w.c) | ((~h->w.b) & h->w.d);
		g = i;
	}
	else if (i < 32)
	{
		f = (h->w.d & h->w.b) | ((~h->w.d) & h->w.c);
		g = (5 * i + 1) % 16;
	}
	else if (i < 48)
	{
		f = h->w.b ^ h->w.c ^ h->w.d;
		g = (3 * i + 5) % 16;
	}
	else
	{
		f = h->w.c ^ (h->w.b | (~h->w.d));
		g = (7 * i) % 16;
	}
	rotate(h, f, w[g], i);
}

t_hash	md5(t_digest_block_getter *reader)
{
	const t_digest_block_descriptor	descriptor = {0, 8, 64, 4};
	uint32_t						i;
	t_hash							final;
	t_hash							h;
	uint32_t						w[16];

	final = (t_hash){.hash_size = 128, {.h128 = {.w = {0x67452301, 0xefcdab89,
		0x98badcfe, 0x10325476}}}};
	while (read_block(&descriptor, reader, w))
	{
		h = final;
		i = -1;
		while (++i < 64)
			iteration(&h.hash.h128, w, i);
		final.hash.h128.w.a += h.hash.h128.w.a;
		final.hash.h128.w.b += h.hash.h128.w.b;
		final.hash.h128.w.c += h.hash.h128.w.c;
		final.hash.h128.w.d += h.hash.h128.w.d;
	}
	return (final);
}
