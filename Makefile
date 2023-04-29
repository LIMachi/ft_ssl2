NAME := ft_ssl
CC := gcc
ITEMS := main alloc_less_argv_parser utils/print_utils utils/endianese digests/block_getter digests/block_getter_constructors digests/common_arguments digests/md5 digests/sha256
SRCDIR := src
OBJDIR := obj
SUBDIRS := utils digests
INCDIRS := inc

NORMINETTE := norminette
CFLAGS += -Wall -Wextra -Werror

DOTC := $(patsubst %, $(SRCDIR)/%.c, $(ITEMS))
DOTO := $(patsubst %, $(OBJDIR)/%.o, $(ITEMS))
OBJSUBDIRS := $(patsubst %, $(OBJDIR)/%, $(SUBDIRS))
INCLUDES := $(patsubst %, -I%, $(INCDIRS))

.PHONY: all clean fclean re norminette

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJSUBDIRS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(DOTO)
	$(CC) $(CFLAGS) $(INCLUDES) $(DOTO) -o $(NAME)

clean:
	rm -f $(DOTO)
	rm -fd $(OBJSUBDIRS)
	rm -fd $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

norminette:
	$(NORMINETTE) $(INCDIRS) $(DOTC)