/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	sha512_expand(uint64_t *w)
{
	uint64_t	i;
	uint64_t	t1;
	uint64_t	t2;

	i = 15;
	while (++i < 80)
	{
		t1 = rot64(w[i - 15], -1) ^ rot64(w[i - 15], -8) ^ (w[i - 15] >> 7);
		t2 = rot64(w[i - 2], -19) ^ rot64(w[i - 2], -61) ^ (w[i - 2] >> 6);
		w[i] = w[i - 16] + t1 + w[i - 7] + t2;
	}
}

void	sha512_iteration(union u_512hash *h, uint64_t *w)
{
	uint64_t	i;
	uint64_t	c;
	uint64_t	t1;
	uint64_t	t2;

	sha512_expand(w);
	i = -1;
	while (++i < 80)
	{
		c = (h->w.e & h->w.f) ^ (~h->w.e & h->w.g);
		t1 = rot64(h->w.e, -14) ^ rot64(h->w.e, -18) ^ rot64(h->w.e, -41);
		t1 += h->w.h + c + sha512_root(i) + w[i];
		c = (h->w.a & h->w.b) ^ (h->w.a & h->w.c) ^ (h->w.b & h->w.c);
		t2 = (rot64(h->w.a, -28) ^ rot64(h->w.a, -34) ^ rot64(h->w.a, -39)) + c;
		h->w.h = h->w.g;
		h->w.g = h->w.f;
		h->w.f = h->w.e;
		h->w.e = h->w.d + t1;
		h->w.d = h->w.c;
		h->w.c = h->w.b;
		h->w.b = h->w.a;
		h->w.a = t1 + t2;
	}
}

int	sha512_complement(uint8_t *buff, size_t read, size_t total_size,
	size_t repeat)
{
	if (repeat == 0)
		buff[read++] = 0x80;
	if (read <= 112)
	{
		if (!little_endian())
			*(__uint128_t *) &buff[112] = (__uint128_t)(total_size * 8);
		else
			*(__uint128_t *) &buff[112] =
				swap_u128((__uint128_t)(total_size * 8));
		return (0);
	}
	return (1);
}

t_hash	sha512(t_bg_reader *reader)
{
	const t_bg_descriptor	descriptor = {8, 16, 1, sha512_complement};
	t_hash					final;
	t_hash					h;
	uint64_t				w[80];
	size_t					i;

	final = (t_hash){.hash_size = 512, {.h512 = {.w = {0x08c9bcf367e6096a,
		0x3ba7ca8485ae67bb, 0x2bf894fe72f36e3c, 0xf1361d5f3af54fa5,
		0xd182e6ad7f520e51, 0x1f6c3e2b8c68059b, 0x6bbd41fbabd9831f,
		0x79217e1319cde05b}}}};
	if (little_endian())
		swap_array(final.hash.h512.b, 8, 8);
	while (read_block(&descriptor, reader, w))
	{
		h = final;
		sha512_iteration(&h.hash.h512, w);
		i = -1;
		while (++i < 8)
			final.hash.h512.u[i] += h.hash.h512.u[i];
	}
	if (little_endian())
		swap_array(final.hash.h512.b, 8, 8);
	return (final);
}
