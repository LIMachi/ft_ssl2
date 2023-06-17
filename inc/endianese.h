/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endianese.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENDIANESE_H
# define ENDIANESE_H

# include <stddef.h>
# include <stdint.h>

int			little_endian(void);
uint32_t	swap_u32(uint32_t v);
uint64_t	swap_u64(uint64_t v);
__uint128_t	swap_u128(__uint128_t v);
void		*swap_array(void *array, size_t length, size_t block);

#endif
