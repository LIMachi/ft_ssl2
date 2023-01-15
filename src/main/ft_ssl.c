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
#include "../digests/digests.h"
#include "ft_ssl.h"

int	debug_digests(t_parser_state *state)
{
	int	i;

	printf("mode: %s, flags: %lx, cinputs: %lu\n",
		modes().modes[state->mode].name, state->flags, state->cinputs);
	i = 0;
	while (i < state->cinputs)
	{
		if (state->inpts[i].type == INPUT_FILE)
			printf("file: %s\n", state->inpts[i].arg);
		else
			printf("string: %s\n", state->inpts[i].arg);
		++i;
	}
	return (0);
}

t_modes	modes(void)
{
	static t_mode	modes[2] = {{"md5", md5}, {"sha256", sha256}};
	static t_modes	out = {2, modes};

	return (out);
}

int	process_error(t_parser_state state, int argc, t_argvp argv,
	t_parse_result result)
{
	printf("got error: %i at argi: %i ('%s')\n", result.err, result.argi,
		argv[result.argi]);
	return (0);
}

int	usage(void)
{
	printf("usage: ft_ssl command [flags] [file/string]\n");
	return (0);
}

int	main(const int argc, t_argvp argv)
{
	t_parser_state	parse_state;
	t_parse_result	result;
	static t_node	node = {INVALID_MODE, NULL, 2, NULL};

	node.choices = (t_choice[2]){
	{'\0', "md5", process_mode, digest_arguments()},
	{'\0', "sha256", process_mode, digest_arguments()}
	};
	parse_state = (t_parser_state){-1, 0, 0,
		malloc(sizeof(t_input) * argc)};
	if (parse_state.inpts == NULL)
		return (-1);
	result = parse(create_parser(argc, argv, &parse_state), &node);
	if (result.err != 0)
		process_error(parse_state, argc, argv, result);
	else if (parse_state.mode != -1)
		result.err = modes().modes[parse_state.mode].run(
				&parse_state, argc - result.argi, &argv[result.argi]);
	else
		usage();
	free(parse_state.inpts);
	return (result.err);
}
