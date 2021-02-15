NAME = philo_three
SRCS = srcs/error.c srcs/ft_utils.c srcs/ft_utils2.c srcs/init_variables.c srcs/launch_philos.c srcs/life.c srcs/main.c srcs/monitoring.c srcs/parsing.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror 
LIB = -lpthread
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
