/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_UTILS_H
# define PRINT_UTILS_H

# include <unistd.h>
# include "ft_ssl.h"

typedef const void	*t_va[];

void	write_maj(int fd, const char *str);
void	write_hash(int fd, t_hash hash);

int		proto_printf(int fd, const char *pattern, t_va inputs);

#endif
