/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	sha384_expand(uint64_t *w)
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

void	sha384_iteration(union u_512hash *h, uint64_t *w)
{
	uint64_t	i;
	uint64_t	c;
	uint64_t	t1;
	uint64_t	t2;

	sha384_expand(w);
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

int	sha384_complement(uint8_t *buff, size_t read, size_t total_size,
	size_t repeat)
{
	if (repeat == 0)
		buff[read++] = 0x80;
	if (read <= 112)
	{
		if (!little_endian())
			*(__uint128_t *) &buff[112] = 8 * (__uint128_t)total_size;
		else
			*(__uint128_t *) &buff[112] =
				swap_u128(8 * (__uint128_t)total_size);
		return (0);
	}
	return (1);
}

t_hash	sha384(t_bg_reader *reader)
{
	const t_bg_descriptor	descriptor = {8, 16, 1, sha384_complement};
	t_hash					final;
	t_hash					h;
	uint64_t				w[80];
	size_t					i;

	final = (t_hash){.hash_size = 384, {.h512 = {.w = {0xd89e05c15d9dbbcb,
		0x07d57c362a299a62, 0x17dd70305a015991, 0x39590ef7d8ec2f15,
		0x310bc0ff67263367, 0x11155868874ab48e, 0xa78ff9640d2e0cdb,
		0xa44ffabe1d48b547}}}};
	if (little_endian())
		swap_array(final.hash.h512.b, 8, 8);
	while (read_block(&descriptor, reader, w))
	{
		h = final;
		sha384_iteration(&h.hash.h512, w);
		i = -1;
		while (++i < 8)
			final.hash.h512.u[i] += h.hash.h512.u[i];
	}
	if (little_endian())
		swap_array(final.hash.h512.b, 8, 8);
	return (final);
}
