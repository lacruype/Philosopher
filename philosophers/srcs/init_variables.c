#include "../includes/philosophers.h"

int		print_nbr(size_t nbr, char *str)
{
	size_t			nnbr;
	long long int	pow;
	char			c;
	int				i;

	i = 0;
	pow = 10;
	nnbr = nbr;
	while (nnbr /= 10)
		pow *= 10;
	while ((pow /= 10))
	{
		c = ((nbr / pow) % 10 + '0');
		str[i++] = c;
	}
	return (i);
}

int		init_philos(t_philosopher **phil, t_init *args)
{
	int i;
	t_philosopher *philos;

	i = 0;
	philos = *phil;
	if (!(philos = malloc(sizeof(t_philosopher) * (args->number_of_philosopher))))
		return (1);
	while (i < args->number_of_philosopher)
	{
		philos[i].arguments = args;
		philos[i].num_philo = (i + 1);
		philos[i].arguments->nb_has_eaten = 0;
		ft_bzero(philos[i].numero_philo, 20);
		print_nbr((i+1), philos[i].numero_philo);
		if (!(philos[i].philo = malloc(sizeof(pthread_t))))
			return (1);
		if (!(philos[i].fork_right = malloc(sizeof(pthread_mutex_t))))
			return (1);
		pthread_mutex_init(philos[i].fork_right, NULL);
		if (i > 0)
			philos[i].fork_left = philos[i - 1].fork_right;
		if (!(philos[i].last_meal = malloc(sizeof(struct timeval))))
			return (1);
		i++;
	}
	philos[0].fork_left = philos[args->number_of_philosopher - 1].fork_right;
	*phil = philos;
	
	return (0);
}

void    init_args(t_init *args)
{
    args->number_of_philosopher = 0;
    args->time_to_die = 0;
    args->time_to_eat = 0;
    args->time_to_sleep = 0;
    args->number_of_time_each_philosophers_must_eat = -1;
	pthread_mutex_init(&args->dead, NULL);
	pthread_mutex_init(&args->lock_status, NULL);
}