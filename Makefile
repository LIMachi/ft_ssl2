NAME := ft_ssl
CC := gcc
ITEMS := src/main.c src/alloc_less_argv_parser.c src/utils/print_utils.c\
	src/utils/endianese.c src/utils/block_getter.c src/utils/bit.c\
	src/digests/common_arguments.c src/digests/md5.c src/digests/sha256.c\
	src/digests/sha224.c src/digests/sha512_roots.c src/digests/sha512.c\
	src/digests/sha384.c
OBJDIR := obj
INCDIRS := inc

NORMINETTE := norminette
CFLAGS += -Wall -Wextra -Werror

DOTO := $(patsubst %.c, $(OBJDIR)/%.o, $(ITEMS))
INCLUDES := $(patsubst %, -I%, $(INCDIRS))

.PHONY: all clean fclean re norminette

all: $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(DOTO)
	$(CC) $(CFLAGS) $(INCLUDES) $(DOTO) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

norminette:
	$(NORMINETTE) $(INCDIRS) $(ITEMS)

test_md5: $(NAME)
	./$(NAME) md5 