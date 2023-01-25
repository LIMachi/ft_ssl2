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

#ifndef COMMON_DIGEST_FUNCTIONS_H
# define COMMON_DIGEST_FUNCTIONS_H

# include "ft_ssl.h"

int		free_state_and_hashes(t_parser_state *state, char **hashes);
int		print_hashes(int read_stdin, t_parser_state *state, char **hashes,
			size_t hash_length);
void	print_stdin(const char *in, char *hash, size_t hash_size, uint64_t flags);
void	print_file_or_string(t_parser_state *state, char *hash,
							 size_t index, size_t hash_length);

#endif
