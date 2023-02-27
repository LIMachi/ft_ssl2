/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_getter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block_getter.h"

t_digest_block_descriptor	descriptor(size_t block_size, size_t word_size,
	size_t size_of_append_size, int big_endian)
{
	return ((t_digest_block_descriptor){big_endian, size_of_append_size,
		block_size, word_size});
}

t_digest_block_getter	str_getter(const char *str)
{
	return ((t_digest_block_getter){0, (union u_digest_target){
		.str = (struct s_str_reader){0, str}}, str_read, 0});
}

t_digest_block_getter	fd_getter(const int fd)
{
	return ((t_digest_block_getter){0, (union u_digest_target){
		.fd = fd}, fd_read, 0});
}
