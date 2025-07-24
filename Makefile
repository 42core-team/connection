LIBRARY = libcon_lib.so

CC = cc
CFLAGS = -Wall -Wextra -Werror -lm -g -fsanitize=address -fPIC

INC = inc
OBJDIR = obj
SRCDIR = src

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(CC) -shared -o $(LIBRARY) $(OBJS) -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(LIBRARY)

re: fclean all

log:
	find inc -type f -print -exec echo "====> {} <====" \; -exec cat {} \; || true
	find src -type f -print -exec echo "====> {} <====" \; -exec cat {} \; || true

.PHONY: all clean fclean re log