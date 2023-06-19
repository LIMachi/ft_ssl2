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
	const t_mode	modes[5] = {{"md5", md5}, {"sha256", sha256},
	{"sha224", sha224}, {"sha512", sha512}, {"sha384", sha384}};

	(void)arg;
	i = -1;
	ps = (t_parser_state *)state;
	while (++i < 5)
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

int	usage(const char *name)
{
	size_t		i;
	const char	*digest_names[] = {"md5", "sha224", "sha256",
		"sha384", "sha512", NULL};

	if (name != NULL)
		proto_printf(1, "sss", (t_va){"usage: ", name,
			" command [flags] [file/string]\n"});
	i = -1;
	write(1, "\nStandard commands:\n\nMessage Digest commands:\n", 46);
	while (digest_names[++i] != NULL)
		proto_printf(1, "ss", (t_va){digest_names[i],
			" [-s/--string <string>] [-p/--stdin-passthrough] [-q/--quiet]"
			"[-r/--reverse-print] [... <file_path>]\n"});
	return (0);
}

int	main(const int argc, t_csa argv)
{
	int							r;
	t_parser_state				parse_state;
	static t_arg_parser_node	node = {FT_SSL_INVALID_MODE, NULL, NULL};

	if (argc <= 1)
		return (usage(argv[0]));
	node.choices = (t_choice_array){
	{0, '\0', "md5", 0, process_mode, digest_arguments(), digest_cleanup},
	{0, '\0', "sha224", 0, process_mode, digest_arguments(), digest_cleanup},
	{0, '\0', "sha256", 0, process_mode, digest_arguments(), digest_cleanup},
	{0, '\0', "sha384", 0, process_mode, digest_arguments(), digest_cleanup},
	{0, '\0', "sha512", 0, process_mode, digest_arguments(), digest_cleanup},
	END_CHOICE};
	parse_state = (t_parser_state){0, 0, {"", NULL}, {}};
	r = parse_argv(argc - 1, &argv[1], &node, &parse_state);
	if (r == -FT_SSL_INVALID_MODE)
	{
		proto_printf(1, "sss", (t_va){"ft_ssl: Error: '", (char *)argv[1],
			"' is an invalid command.\n"});
		usage(NULL);
	}
	return (0);
}
