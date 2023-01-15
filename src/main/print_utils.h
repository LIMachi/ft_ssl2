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

void	write_all_first_line(int fd, const char *str);
void	write_maj(int fd, const char *str);
int		print_file_error(t_parser_state *state, size_t i);

#endif
