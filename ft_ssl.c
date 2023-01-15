//#include <malloc.h>
//#include <fcntl.h>
//#include "ft_ssl.h"
//#include "unistd.h"

/**
* structure:
* each algorithm is a sub program (pointer to function style)
* man/input/output is handled by the main executable
* parameters of the input are strict by default (no mingle of -parameters and files)
*/

/*
uint64_t flag(char c) {
	if (c >= '0' && c <= '9')
		return ((uint64_t)1) << (c - '0');
	if (c >= 'a' && c <= 'z')
		return ((uint64_t)1) << (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return ((uint64_t)1) << (c - 'A' + 36);
	return 0;
}*/

/*
void default_printer(const t_args *input, const char *algorithm_name, char *hash[]) {
	size_t h = 0;
	size_t i = 0;
	if (input->piped != NULL) {
		if (IS_FLAG_M(input->flags, 'q')) {
			if (IS_FLAG_M(input->flags, 'p')) {
				print_all(input->piped);
				write(1, "\n", 1);
			}
			print_all(hash[0]);
		} else {
			write(1, "(", 1);
			if (IS_FLAG_M(input->flags, 'p')) {
				write(1, "\"", 1);
				print_all(input->piped);
				write(1, "\")= ", 4);
			} else
				write(1, "stdin)= ", 8);
			print_all(hash[0]);
		}
		write(1, "\n", 1);
		h = 1;
	}
	while (input->payload[i].type != END) {

		++h;
		++i;
	}
}

int simple_str_cmp(const char *str1, const char *str2) {
	if (str1 == NULL || str2 == NULL) return 0;
	int i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
		++i;
	return str1[i] == '\0' && str2[i] == '\0';
}

char *append(char *to, size_t size, const char *from, size_t add) {
	char *out = malloc(size + add);
	if (to != NULL)
		for (size_t i = 0; i < size; ++i)
			out[i] = to[i];
	for (size_t i = 0; i < add; ++i)
		out[size + i] = from[i];
	if (to != NULL)
		free(to);
	return out;
}

char *read_file(int fd) {
	char buff[128];
	char *out = NULL;
	size_t s = 0;
	size_t r;
	while ((r = read(fd, buff, 128)) > 0) {
		out = append(out, s, buff, r);
		s += r;
	}
	return out;
}
*/
/**
* completely clear (free + reset to NULL) the input (if not null) and the list of hash (if not null)
*/
/*
void clear_all(t_args *input, char *hash[]) {
	if (input != NULL) {
		if (input->piped != NULL){
			free(input->piped);
			input->piped = NULL;
		}
		for (int i = 0; input->payload[i].type != END; ++i){
			free(input->payload[i].content);
			input->payload[i].content = NULL;
			free(input->payload[i].name);
			input->payload[i].name = NULL;
			input->payload[i].type = END;
		}
		free(input->payload);
		input->payload = NULL;
		input->flags = 0;
	}
	if (hash != NULL) {
		for (int i = 0; hash[i] != NULL; ++i) {
			free(hash[i]);
			hash[i] = NULL;
		}
		free(hash);
	}
}*/

/**
* expected input order: <cmd> <flags> <files/string>
* return index of algorithm and updated input
* return -1 and print an err on invalid input
*/
/*
int extract_input(int argc, const char *const *argv, const t_algorithm algorithms[], t_args *input) {
	input->piped = NULL;
	input->payload = NULL;
	input->flags = 0;

	if (argc < 2) {
		print_usage();
		return -1;
	}

	int found_index = 0;
	while (!simple_str_cmp(algorithms[found_index].name, argv[1])) {
		if (algorithms[found_index].name == NULL){
			print_invalid_command(argv[1]);
			return -1;
		}
		++found_index;
	}
	int count_options = 0;
	while (count_options + 2 < argc && argv[count_options + 2][0] == '-'){
		uint64_t f = flag(argv[count_options + 2][1]);
		if ((f | algorithms[found_index].valid_flags) == 0) {
			print_invalid_option(algorithms[found_index].name, argv[2 + count_options]);
			return -1;
		}
		++count_options;
	}

	int count_files = argc - 2 - count_options;
	input->payload = malloc(sizeof(t_payload*) * (count_files + 1));
	int i = 0;
	while (i < count_files) {
		input->payload[i] = (t_payload){END, NULL, NULL};
		if (argv[2 + count_options + i][0] == '-') {
			clear_all(input, NULL);
			print_invalid_option(algorithms[found_index].name, argv[2 + count_options + i]);
			return -1;
		}
		const char *arg = argv[2 + count_options + i];
		input->payload[i].name = arg;
		if (isAlreadyStrEscaped(arg)){
			input->payload[i].content = append(NULL, 0, &arg[1], len(arg) - 2);
			input->payload[i].type = STRING;
		} else {
			int fd = open(arg, O_RDONLY);
			if (fd == -1) {
				clear_all(input, NULL);
				print_invalid_option(algorithms[found_index].name, arg);
				return -1;
			}
			input->payload[i].content = read_file(fd);
			input->payload[i].type = READ_FILE;
			close(fd);
		}
		++i;
	}

	algorithms[found_index].read_stdin(input);

	if (input->payload[0].type == END && input->piped == NULL) {
		clear_all(input, NULL);
		print_usage();
		return -1;
	}

	return found_index;
}

void default_read_stdin(t_args *input) {
	if ((input->flags & flag('p')) || input->payload[0].type == END)
		input->piped = read_file(0);
	else
		input->piped = NULL;
}*/

/*
void md5(const t_args *input, void (*printer)(const t_args *input, const char *algorithm_name, char *hash[])) {
	char *test[] = {"test"};
	printer(input, "MD5", test);
}

void sha256(const t_args *input, void (*printer)(const t_args *input, const char *algorithm_name, char *hash[])) {
}
*/

/*
int main(int argc, const char *const *argv) {
	const t_algorithm algorithms[] = {
		{"md5", DEFAULT_FLAGS, default_read_stdin, md5, default_printer},
		{"sha256", DEFAULT_FLAGS, default_read_stdin, sha256, default_printer },
		{NULL, 0, NULL, NULL}};

	t_args input;
	int found_index;

	found_index = extract_input(argc, argv, algorithms, &input);
	if (found_index == -1)
		return 0;

	algorithms[found_index].algorithm(&input, algorithms[found_index].printer);
}
*/
