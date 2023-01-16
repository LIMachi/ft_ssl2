/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argument_parser.h"

/**
* 'single' (also need a header) C file varg parser by Hugo Martzolf
* 42's norm compliant (version 3.3.51 https://github.com/42School/norminette)
*
* To use, simply create a parser tree (each node represent a set of choices for
* a state machine) and use the parse_varg function.
*
* A node is a struct of 4 variables:
* An optional (0 to ignore) error value if no choices matched at this node.
* A nullable next node to jump to if no choices matched.
* The number of choices at this node.
* The choices at this node.
*
* A choice is a struct of 4 variables:
* An alias (single char) used for short form arguments (-a -> alias 'a'), set to
*   '\0' to ignore. Partially GNU compatible (-ab -> aliases 'a' & 'b', but if
*   'a' consume the argument after it 'b' will be skipped).
* A fully qualified name (no assumption of using '--' before, '--help' and
*   'mode' are both valid).
* A nullable callback that will allow you to process the current node how you
*   see fit. The parameters give you immutable accesses to the node that
*   matched, the current and next arg, a flag to signal that you already
*   processed the next arg, a way to send an error and a pointer defined
*   by the user at the start of the parsing. Return 0 to continue the processing
*   of arguments or !0 to stop the parser at the current argument.
* A nullable reference to a node to jump to if this choice matched and the
*   callback returned 0.
*
* Once you have your tree setup, you can call parse_varg with argc and argv as
* the first 2 parameters (the first 2 parameters of your main), then you can
* pass a pointer to your data that will be passed to the callbacks of matching
* choices, finally add the first node of your tree and your set.
* The returned value is a pair of error value (0 means no error) and at what
* argv index the parsing stopped (potentially stopped at index == argc).
*
* Example: (t_argvp is a helper type to represent a immutable char **)
*
* int main(const int argc, t_argvp argv) {
*   t_parser_node node = {INVALID_CHOICE, NULL, 2, (t_choice[2]){
*     {'\0', "hello", NULL, &node}, {'\0', "world", NULL, &node}}};
*
*   return parse(argc, argv, NULL, &node);
* }
* ^
* the above code will result in a parsing that will allow only 'hello' and
* 'world' as valid parameters and will return INVALID_CHOICE if an arg does not
* match.
*
* int main(const int argc, t_argvp argv) {
*   t_state state = {0};
*
*   t_parser_node f = {0, NULL, 4, (t_choice[4){
*     {'s', NULL, process_string, &f},
*     {'p', NULL, process_flag, &f},
*     {'q', NULL, process_flag, &f},
*     {'r', NULL, process_flag, &f}};
*
*   t_parser_node modes = {INVALID_MODE, NULL, 2, (t_choice[2]){
*     {'\0', "md5", got_mode, &f}, {'\0', "sha256", got_mode, &f}}};
*
*   t_parse_result res = parse(argc, argv, &state, &modes);
*   ... do something with res.argi, argc, argv and state
* }
* ^
* the above example is a little more representative:
* the program requires one of 'md5' or 'sha256' as its first argument,
* then will accept a random order of optional flags (pqr) and
* string consumers (s) (process_string and process_flag are user defined
* functions, they are not supplied with the parser. imagine that process_flag
* will update an int in the state structure and process_string will update a
* list of pointers in the state).
* ->
* ./run md5 -s test -q -> valid
* ./run md5 -rsq test -> valid, but q will not be caught
* ./run test md5 -> invalid, first arg is not one of 'md5' or 'sha256'
* ./run md5 file1 file2 -> valid, no flags or string will be processed but with
*   argv[res.argi] you can access 'file1' and by comparing argi and argc, you
*   can iterate on the files with ease.
* ./run md5 test -q -> will stop after md5 since 'test' is not known
*/

static inline int	consume(t_parse_result *const out, t_parser parser,
	int *i, const t_node **const node)
{
	const char	*arg;
	int			consume_arg;

	if (i[0] < 0)
		out->err = (*node)->missing_error;
	if (i[0] == -1)
		++out->argi;
	if (i[0] < 0)
		return (1);
	if (out->argi + 1 < parser.argc)
		arg = parser.argv[out->argi + 1];
	else
		arg = NULL;
	consume_arg = 0;
	if ((*node)->choices[i[0]].consume != NULL && (*node)->choices[i[0]]
		.consume(&(*node)->choices[i[0]], (t_consume_params){arg, &consume_arg},
		&out->err, parser.state))
		return (1);
	if ((*node)->choices[i[0]].next != NULL)
		*node = (*node)->choices[i[0]].next;
	if (consume_arg)
		out->argi += 1 + (i[1] > 0);
	if (consume_arg)
		i[1] = 0;
	return (0);
}

/**
* `*ai = *ai + (arg[*ai] != '\0') - *ai * (arg[*ai] == '\0');` and
* `return (i - (i + 2) * (*ai > 0));`
* are logical + mathematical work around not being allowed to juste use
* ternaries to do `*ai = arg[*ai] == '\0' ? 0 : *ai + 1;` and
* `return (*ai > 0 ? -2 : i);`
*/

static inline int	match_alias_or_name(const char *arg,
	t_parse_result *out, int *ai, const t_node *node)
{
	int	i;

	if (*ai == 0 && arg[0] == '-' && arg[1] != '-')
		*ai = 1;
	i = 0;
	while (i < node->choice_count && !str_match(arg, node->choices[i].name))
		++i;
	if (i < node->choice_count || (*ai > 0 && arg[*ai] == '\0'))
	{
		*ai = 0;
		++out->argi;
	}
	if (*ai > 0)
	{
		i = 0;
		while (i < node->choice_count && arg[*ai] != node->choices[i].alias)
			++i;
		if (i < node->choice_count)
		{
			*ai = *ai + (arg[*ai] != '\0') - *ai * (arg[*ai] == '\0');
			return (i);
		}
	}
	return (i - (i + 2) * (*ai > 0));
}

t_parse_result	parse_varg(const int argc, t_argvp varg, void *state,
	const t_node *node)
{
	t_parse_result	out;
	int				i[2];

	out = (t_parse_result){0, 1};
	i[1] = 0;
	while (out.argi < argc && node != NULL)
	{
		if (varg[out.argi][0] == '-' && varg[out.argi][1] == '-'
				&& !varg[out.argi][2])
			i[0] = -1;
		else
			i[0] = match_alias_or_name(varg[out.argi], &out, &i[1], node);
		if (i[0] >= (int)node->choice_count)
		{
			out.err = node->missing_error;
			if (out.err != 0)
				return (out);
			node = node->next;
		}
		else if (consume(&out, (t_parser){argc, varg, state}, i, &node))
			return (out);
	}
	return (out);
}
