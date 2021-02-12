#include "../includes/philosophers.h"


void	ft_print_args(t_init *args)
{
	ft_putnbr_fd(args->number_of_philosopher, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(args->time_to_die, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(args->time_to_eat, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(args->time_to_sleep, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(args->number_of_time_each_philosophers_must_eat, 1);
	ft_putchar_fd('\n', 1);
}