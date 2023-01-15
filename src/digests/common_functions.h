/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_functions.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_FUNCTIONS_H
# define COMMON_FUNCTIONS_H

# include "../main/ft_ssl.h"

int		free_state_and_hashes(t_parser_state *state, char **hashes);
int		print_hashes(int read_stdin, t_parser_state *state, char **hashes, size_t hash_length);

#endif
