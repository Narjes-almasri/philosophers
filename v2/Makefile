NAME	= philo
CFLAGS	= -Werror -Wall -Wextra

SRC = init.c \
      main.c \
      monitor_simulation.c \
      output.c \
      philo.c \
      time.c \
      utils.c \
      utils2.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
