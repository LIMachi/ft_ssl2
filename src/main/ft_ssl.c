/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <unistd.h>
#include "digests.h"
#include "ft_ssl.h"

t_modes	modes(void)
{
	static t_mode	modes[2] = {{"md5", DIGEST, md5},
	{"sha256", DIGEST, sha256}};
	static t_modes	out = {2, modes};

	return (out);
}

int	usage(void)
{
	write(1, "Usage: ft_ssl command [flags] [file/string]\n", 44);
	return (0);
}

int	main(const int argc, t_csa argv)
{
	int							r;
	t_parser_state				parse_state;
	static t_arg_parser_node	node = {FT_SSL_INVALID_MODE, NULL, 2, NULL};

	node.choices = (t_arg_parser_choice [2]){
	{0, '\0', "md5", 0, process_mode, digest_arguments()},
	{0, '\0', "sha256", 0, process_mode, digest_arguments()}};
	parse_state = (t_parser_state){-1, 0, 0, NULL};
	r = parse_argv(argc - 1, &argv[1], &node, &parse_state);
	if (parse_state.processed == 0 && parse_state.mode >= 0
		&& parse_state.mode <= 1)
		process_stdin(NULL, NULL, &parse_state);
	return (0);
}
