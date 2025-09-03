NAME	= philo
CFLAGS	= -Werror -Wall -Wextra

SRC = ini.c \
      main.c \
      monitor_and_sim.c \
      prints.c \
      philos.c \
      sim.c \
      routine.c \
      checking.c\
      clean.c

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
