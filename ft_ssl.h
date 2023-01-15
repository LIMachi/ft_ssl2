/*#ifndef FT_SSL_H
# define FT_SSL_H
#include <stdint.h>

typedef enum e_type t_type;
typedef struct s_payload t_payload;
typedef struct s_args t_args;
typedef struct s_algorithm t_algorithm;

enum e_type {
	END,
	STRING,
	READ_FILE
};

#define FLAG_M(x) (((uint64_t)1) << (x - 'a' + 10))
#define IS_FLAG_M(v, x) ((v & FLAG_M(x)) != 0)

const uint64_t DEFAULT_FLAGS = FLAG_M('p') | FLAG_M('q') | FLAG_M('r') | FLAG_M('s');

struct s_payload {
	t_type type;
	char *name;
	char *content;
};

struct s_args {
	uint64_t flags;
	char *piped;
	t_payload *payload;
};

struct s_algorithm {
	const char *name;
	uint64_t valid_flags;
	void (*const read_stdin)(t_args *input);
	void (*const algorithm)(const t_args *input, void (*const printer)(const t_args *input, const char *algorithm_name, char *hash[]));
	void (*const printer)(const t_args *input, const char *algorithm_name, char *hash[]);
};

int len(const char *string);
int isAlreadyStrEscaped(const char *string);
void print_usage();
void print_invalid_command(const char *string);
void print_invalid_option(const char *cmd, const char *string);
void print_all(const char *string);

#endif
*/
