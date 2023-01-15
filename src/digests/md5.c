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

#include <malloc.h>
#include <string.h>
#include "../main/ft_ssl.h"
#include "../main/compact_array.h"
#include "../main/print_utils.h"
#include "common_functions.h"
#include "digests.h"

void		left_rotate_b(t_md5hash *hash, uint32_t f, uint32_t w, uint32_t i) {
	uint32_t		x;
	uint32_t		shift;
	static uint32_t	usine[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
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
	static uint32_t	ushift[16] = {7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6,
		10, 15, 21};

	shift = ushift[i & 3 | ((i & 0xF0) >> 2)];
	x = hash->w.a + f + usine[i] + w;
	hash->w.b += (x << shift) | (x >> (32 - shift));
}

uint32_t	get_message_block(const char *in, size_t in_len, uint32_t word_index) {
	t_md5hash t;
	t_md5hash s;

	if (word_index * 4 + 4 <= in_len)
		return ((uint32_t*)in)[word_index];
	t = (t_md5hash){.u128 = 0};
	for (uint32_t i = 0; i < 4; ++i) {
		if (i + word_index * 4 < in_len)
			t.b[i] = in[i + word_index * 4];
		else if (i + word_index * 4 == in_len)
			t.b[i] = 0x80;
		else if ((i + word_index * 4) % 64 < 56)
			t.b[i] = 0;
		else {
			s = (t_md5hash){.u64 = {in_len * 8, 0}};
			t.b[i] = s.b[(i + word_index * 4) % 64 - 56];
		}
	}
	return t.w.a;
}

//md5sum test.txt -> 35310a49e26265a58122d9c6e0823f49
//c                  35310a49e26265a58122d9c6e0823f49

void	md5h(const char *in, char *out)
{
	uint8_t *msg = NULL;

	uint32_t h0, h1, h2, h3;

    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
    size_t	initial_len = 0;
	while (in[initial_len] != '\0')
		++initial_len;

    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

	msg = calloc(new_len + 64, 1);

	memcpy(msg, in, initial_len);
	msg[initial_len] = 128;
	uint32_t bits_len = 8*initial_len;
	memcpy(msg + new_len, &bits_len, 4);

	uint8_t *test = calloc(new_len + 64, 1);
	for (int i = 0; i <= new_len / 4; ++i){
		t_md5hash	th = {.w = get_message_block(in, initial_len, i)};
		test[i * 4] = th.b[0];
		test[i * 4 + 1] = th.b[1];
		test[i * 4 + 2] = th.b[2];
		test[i * 4 + 3] = th.b[3];
	}

	int offset;
    for(offset=0; offset<new_len; offset += (512/8)) {

		t_md5hash h = {.w = {h0, h1, h2, h3}};

        uint32_t i;
        for(i = 0; i<64; i++){
			uint32_t f, g;
			if (i < 16){
				f = (h.w.b & h.w.c) | ((~h.w.b) & h.w.d);
				g = i;
			} else if (i < 32){
				f = (h.w.d & h.w.b) | ((~h.w.d) & h.w.c);
				g = (5 * i + 1) % 16;
			} else if (i < 48){
				f = h.w.b ^ h.w.c ^ h.w.d;
				g = (3 * i + 5) % 16;
			} else{
				f = h.w.c ^ (h.w.b | (~h.w.d));
				g = (7 * i) % 16;
			}
			uint32_t temp = h.w.d;
			h.w.d = h.w.c;
			h.w.c = h.w.b;
			left_rotate_b(&h, f, get_message_block(in, initial_len, g), i);
			h.w.a = temp;
		}

		h0 += h.w.a;
        h1 += h.w.b;
        h2 += h.w.c;
        h3 += h.w.d;
	}

	t_md5hash final = (t_md5hash){.w = {h0, h1, h2, h3}};
    for (int i = 0; i < 16; ++i)
    	out[i] = final.b[i];
}

/**
* thanks to a bug in the norminette, when a if statement is the only line of any
* block keyword that accept both single lines or blocks (like while, if, for,
* etc...), the 'else' keyword of the if statement is seen (by norminette) as
* being outside the block keyword, and should follow the indentation of the
* block keyword and not the indentation of the previous if keyword... *facepalm*
*/

int	md5(t_parser_state *state, int argc, t_argvp argv)
{
	int		read_stdin;
	char	**hashes;
	size_t	i;

	get_remainder_files(state, argc, argv);
	read_stdin = state->input_count == 0;
	read_stdin |= (state->flags & flag('p')) != 0;
	i = -1;
	while (++i < state->input_count)
		if (state->inputs[i].type == INPUT_FILE)
			state->inputs[i].data = read_file(state->inputs[i].arg);
	else
		state->inputs[i].data = state->inputs[i].arg;
	if (read_stdin)
		state->inputs[state->input_count++] = (t_parser_input){STDIN, "stdin",
			read_fd(0)};
	hashes = allocate_compact_2d_array(state->input_count, sizeof(char) * 16);
	i = -1;
	while (++i < state->input_count)
		if (state->inputs[i].data == NULL)
			return (print_file_error(state, i)
				| free_state_and_hashes(state, hashes));
	else
		md5h(state->inputs[i].data, hashes[i]);
	return (print_hashes(read_stdin, state, hashes, 16));
}
