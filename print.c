/*#include <unistd.h>

int len(const char *string) {
	if (string == NULL)
		return 0;
	int c = 0;
	while (string[c] != '\0')
		++c;
	return c;
}

int isAlreadyStrEscaped(const char *string) {
	if (string == NULL)
		return 0;
	int ok = (string[0] == '\'') + 2 * (string[0] == '"');
	if (!ok)
		return 0;
	int c = 0;
	while (string[c] != '\0')
		++c;
	if (c < 2)
		return 0;
	return ok == (string[c - 1] == '\'') + 2 * (string[c - 1] == '"');
}

void writeEscaped(const char *string) {
	int esc = isAlreadyStrEscaped(string);
	if (!esc)
		write(1, "'", 1);
	write(1, string, len(string));
	if (!esc)
		write(1, "'", 1);
}

void print_usage() {
	write(1, "usage: ft_ssl command [flags] [file/string]\n", 44);
}

void print_invalid_command(const char *string) {
	write(1, "ft_ssl: Error: ", 15);
	writeEscaped(string);
	write(1, " is an invalid command.\n", 24);
}

void print_invalid_option(const char *cmd, const char *string) {
	write(1, "ft_ssl: ", 8);
	write(1, cmd, len(cmd));
	write(1, ": ", 2);
	write(1, string, len(string));
	write(1, ": No such file or directory\n", 28);
}

void print_all(const char *string) {
	write(1, string, len(string));
}
*/
