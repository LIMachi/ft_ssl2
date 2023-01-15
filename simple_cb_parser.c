/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cb_parser.choices                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include "src/argument_parser/argument_parser.h"
*/

/*
t_parse_result parse(int argc, char **argv, t_node *parser, void *state) {
	t_parse_result out;
	size_t ci;

	out = (t_parse_result){0, 1};
	while (out.argi < argc && parser != NULL) {
		ci = 0;
		while (ci < parser->choice_count && !((argv[out.argi][0] == '-'
											   && argv[out.argi][1] == parser->choices[ci].alias)
											  || str_match(argv[out.argi], parser->choices[ci].name)))
			++ci;
		if (ci < parser->choice_count) {
			if (parser->choices[ci].callback != NULL && parser->choices[ci].callback(&parser->choices[ci],
																					 &out, out.argi + 1 < argc ? argv[out.argi + 1] : NULL, state))
				return out;
			if (parser->choices[ci].next != NULL)
				parser = parser->choices[ci].next;
		} else {
			if ((out.err = parser->missing_error) != 0)
				return out;
			parser = parser->next;
		}
		if (parser != NULL)
			++out.argi;
	}
	return out;
}

int end_parse(int argc, char **argv, int error, int argv_index, uint64_t flags) {
	printf("argc: %i, argv[0]: %s, err: %i, argv_index: %i, flags: %lx, rem: '%s'\n", argc, argv[0], error, argv_index, flags, argv_index < argc ? argv[argv_index] : "");
	if (error == 1)
		printf("ft_ssl: Error: Missing argument after: '%s'\n", argv[argv_index]);
	else if (error == 2) {
		printf("ft_ssl: Error: '%s' is and invalid command.\n\nCommands:\nmd5\nsha256\nbase64\n\nFlags:\n-p -q -r -s\n", argv[argv_index]);
	}
	else if (argc < 2)
		printf("usage: ft_ssl command [flags] [file/string]\n");
	else
		for (int i = argv_index; i < argc; ++i) {
			int fd = open(argv[i], O_RDONLY);
			if (fd == -1)
				printf("ft_ssl: Error: Can't open file '%s'\n", argv[i]);
			else {
				printf("file is ok: '%s'\n", argv[i]);
				close(fd);
			}
		}
	return 0;
}

int consume_mode(t_choice *self, t_parse_result *result, char *arg, void *state) {
	printf("consuming mode: '%s'\n", self->name);
	if (str_match("md5", self->name))
		*(uint64_t*)state |= ((uint64_t)1) << 42;
	if (str_match("sha256", self->name))
		*(uint64_t*)state |= ((uint64_t)2) << 42;
	if (str_match("base64", self->name))
		*(uint64_t*)state |= ((uint64_t)3) << 42;
	return 0;
}

int consume_flag(t_choice *self, t_parse_result *result, char *arg, void *state) {
	printf("consuming flag: '%choices'\n", self->alias);
	if (self->alias >= 'a' && self->alias <= 'z')
		*(uint64_t*)state |= 1 << (self->alias - 'a');
	return 0;
}

int consume_string(t_choice *self, t_parse_result *result, char *arg, void *state) {
	printf("consuming string: '%s'\n", arg);
	if (arg != NULL)
		result->argi++;
	return result->err = arg == NULL;
}*/

/*
int main(int argc, char **argv) {
	t_node digest_args = {0, NULL, 4, (t_choice[4]){
		{'s', NULL, consume_string, &digest_args},
		{'p', NULL, consume_flag, &digest_args},
		{'q', NULL, consume_flag, &digest_args},
		{'r', NULL, consume_flag, &digest_args},
	}};
	t_node base64_args = {0, NULL, 4, (t_choice[4]){
		{'d', NULL, consume_flag, &digest_args},
		{'e', NULL, consume_flag, &digest_args},
		{'i', NULL, consume_string, &digest_args},
		{'o', NULL, consume_string, &digest_args},
	}};
	t_node modes = {2, NULL, 3, (t_choice[3]){
		{'\0', "md5", consume_mode, &digest_args},
		{'\0', "sha256", consume_mode, &digest_args},
		{'\0', "base64", consume_mode, &base64_args}
	}};
	uint64_t state = 0;
	t_parse_result r = parse(argc, argv, &modes, &state);
	return end_parse(argc, argv, r.err, r.argi, state);
}
*/
