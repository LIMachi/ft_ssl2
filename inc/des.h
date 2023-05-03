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

#ifndef DES_H
# define DES_H

# include <stddef.h>
# include <stdint.h>

uint64_t	des_initial_permutation(uint64_t v, int inverse);
void		des_initial_key_permutation(uint64_t key, uint32_t ks[2]);
void		des_initialize_sub_keys(uint32_t ks[2], uint64_t sub_keys[16]);
uint32_t	des_box(size_t box_index, uint8_t row, uint8_t column);

#endif
