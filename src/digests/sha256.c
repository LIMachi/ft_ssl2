/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint32_t	root(uint32_t i)
{
	static const uint32_t	out[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf,
		0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98,
		0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
		0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
		0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8,
		0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
		0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e,
		0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
		0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c,
		0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee,
		0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
		0xc67178f2};

	return (out[i]);
}

uint32_t	rr(uint32_t w, uint32_t s)
{
	return ((w >> s) | (w << (32 - s)));
}

void	expand(uint32_t *w)
{
	uint32_t	i;
	uint32_t	t1;
	uint32_t	t2;

	i = 15;
	while (++i < 64)
	{
		t1 = rr(w[i - 15], 7) ^ rr(w[i - 15], 18) ^ (w[i - 15] >> 3);
		t2 = rr(w[i - 2], 17) ^ rr(w[i - 2], 19) ^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + t1 + w[i - 7] + t2;
	}
}

void	sha256_iteration(union u_256hash *h, uint32_t *w)
{
	uint32_t	i;
	uint32_t	c;
	uint32_t	t1;
	uint32_t	t2;

	expand(w);
	i = -1;
	while (++i < 64)
	{
		c = (h->w.e & h->w.f) ^ (~h->w.e & h->w.g);
		t1 = rr(h->w.e, 6) ^ rr(h->w.e, 11) ^ rr(h->w.e, 25);
		t1 += h->w.h + c + root(i) + w[i];
		c = (h->w.a & h->w.b) ^ (h->w.a & h->w.c) ^ (h->w.b & h->w.c);
		t2 = (rr(h->w.a, 2) ^ rr(h->w.a, 13) ^ rr(h->w.a, 22)) + c;
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

t_hash	sha256(t_block_getter *reader)
{
	const t_block_descriptor	descriptor = {1, 8, 64, 4, 0x80, 0};
	t_hash						final;
	t_hash						h;
	uint32_t					w[64];
	size_t						i;

	final = (t_hash){.hash_size = 256, {.h256 = {.w = {0x67e6096a, 0x85ae67bb,
		0x72f36e3c, 0x3af54fa5, 0x7f520e51, 0x8c68059b, 0xabd9831f,
		0x19cde05b}}}};
	if (little_endian())
		swap_array(final.hash.h256.b, 8, 4);
	while (read_block(&descriptor, reader, w))
	{
		h = final;
		sha256_iteration(&h.hash.h256, w);
		i = -1;
		while (++i < 8)
			final.hash.h256.aw[i] += h.hash.h256.aw[i];
	}
	if (little_endian())
		swap_array(final.hash.h256.b, 8, 4);
	return (final);
}
