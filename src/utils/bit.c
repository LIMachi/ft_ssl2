/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bit.h"

/**
* rotate bits of a value left/right (like a shift, but over/underflowing bits
* are reinserted)
* @param value to be rotated
* @param shift > 0 => rotate left; < 0 rotate right
* @return the rotated value
*/

uint32_t	rot32(uint32_t value, ssize_t shift)
{
	if (shift < 0)
		return ((value >> -shift) | (value << (32 + shift)));
	else
		return ((value << shift) | (value >> (32 - shift)));
}
