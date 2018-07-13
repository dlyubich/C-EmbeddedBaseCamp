b = \033[0;36m
r = \033[0;34m
g = \033[0;32m
e = \033[0m

NAME=liblst.so
SRC = lst_clear.c \
	lst_count.c \
	lst_insert.c \
	lst_remove.c \
	lst_reverse.c \
	lst_tailadd.c \
	lst_headadd.c \
	lst_new.c \
	lst_sort.c \
	lst_traverse.c \
	lst_extend.c
	
OBJ = $(SRC:.c=.o)
CFLAGS = -fPIC -Wall -O2 -g
LDFLAGS = -shared
CC = -gcc

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e '$bLIBLST : Sources compiling...$e'
	$(CC) ${LDFLAGS} -o $@ $^
	@echo -e '$gLIBLST : Compile with SUCCESS!$e'

$(SRC:.c=.o):%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)
	@echo -e '$rObjects removed...$e'

fclean: clean
	rm -f $(NAME)
	@echo -e '$rLIBLST binary removed...$e'

re: fclean all
