/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "print_utils.h"

static inline int	streq(const char *s1, const char *s2)
{
	size_t	i;

	if (s1 == s2 && s1 != NULL)
		return (1);
	if (s1 == NULL || s2 == NULL)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		++i;
	return (s1[i] == s2[i]);
}

unsigned int	process_mode(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	size_t			i;
	t_parser_state	*ps;
	static t_mode	modes[2] = {{"md5", md5, digest_cleanup},
	{"sha256", sha256, digest_cleanup}};

	(void)arg;
	i = -1;
	ps = (t_parser_state *)state;
	while (++i < 2)
	{
		if (streq(modes[i].name, self->label))
		{
			ps->mode = modes[i];
			return (0);
		}
	}
	return (FT_SSL_INVALID_MODE);
}

unsigned int	process_flag(const t_arg_parser_choice *const self,
	const char *const arg, void *state)
{
	(void)arg;
	((t_parser_state *)state)->flags |= ((uint64_t)1) << (self->alias - 'a');
	return (0);
}

int	default_cleanup(void *state, int ret, const int argc, t_csa argv)
{
	(void)state;
	if (argc <= 1)
		write(1, "usage: ft_ssl command [flags] [file/string]\n", 44);
	else if (ret == -FT_SSL_INVALID_MODE)
		proto_printf("sss", (const void *[3]){"ft_ssl: Error: '", argv[1],
			"' is an invalid command.\n"});
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
	parse_state = (t_parser_state){0, 0, {"", NULL, default_cleanup}};
	r = parse_argv(argc - 1, &argv[1], &node, &parse_state);
	return (parse_state.mode.cleanup(&parse_state, r, argc, argv));
}