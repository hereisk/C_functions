CC = gcc
CFLAGS  = -Wall -Werror -Wextra -g3
SANITIZE = -fsanitize=address
MAIN = tar
SRCS = $(wildcard my_tar/*.c)
OBJS = $(SRCS:.c=.o)

ALL: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $(LINKERFLAG) $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(MAIN)

re: fclean all
