/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compact_array.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPACT_ARRAY_H
# define COMPACT_ARRAY_H

# include <stddef.h>

void	*allocate_compact_2d_array(size_t x, size_t y);

#endif
