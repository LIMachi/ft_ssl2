/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digests.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIGESTS_H
# define DIGESTS_H

# include <stdint.h>
# include "alloc_less_argv_parser.h"
# include "block_getter.h"

typedef struct s_hash	t_hash;

struct					s_512words {
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
	uint32_t	e;
	uint32_t	f;
	uint32_t	g;
	uint32_t	h;
	uint32_t	i;
	uint32_t	j;
	uint32_t	k;
	uint32_t	l;
	uint32_t	m;
	uint32_t	n;
	uint32_t	o;
	uint32_t	p;
};

struct					s_256words {
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
	uint32_t	e;
	uint32_t	f;
	uint32_t	g;
	uint32_t	h;
};

struct					s_128words {
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
};

union					u_512hash {
	uint64_t			u[8];
	struct s_512words	w;
	uint32_t			aw[16];
	unsigned char		b[64];
};

union					u_256hash {
	uint64_t			u[4];
	struct s_256words	w;
	uint32_t			aw[8];
	unsigned char		b[32];
};

union					u_128hash {
	uint64_t			u[2];
	struct s_128words	w;
	uint32_t			aw[4];
	unsigned char		b[16];
};

union					u_hash {
	union u_128hash		h128;
	union u_256hash		h256;
	union u_512hash		h512;
};

struct					s_hash {
	size_t			hash_size;
	union u_hash	hash;
};

typedef t_hash			(*t_hasher)(t_digest_block_getter *reader);

t_arg_parser_node		*digest_arguments(void);

#endif
