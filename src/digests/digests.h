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

# include "../main/ft_ssl.h"

typedef union u_md5hash	t_md5hash;

struct					s_words {
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
};

union					u_md5hash {
	uint64_t			u64[2];
	struct s_words		w;
	char				b[16];
};

t_node					*digest_arguments(void);

#endif
