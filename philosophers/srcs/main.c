#include "../includes/philosophers.h"

static inline void	status_philo(t_philosopher *philos, char *msg)
{
	struct timeval now;
	int i;
	int j;
	char str[64];
	long long number;

	pthread_mutex_lock(&philos->arguments->lock_status);
	gettimeofday(&now, NULL);
	number = ((now.tv_sec - philos->arguments->start_philo.tv_sec) * 1000 + (now.tv_usec - philos->arguments->start_philo.tv_usec) * 0.001);
	i = print_nbr(number, str);
	j = 0;
	str[i++] = '\t';

	while (philos->numero_philo[j])
		str[i++] = philos->numero_philo[j++];
	str[i++] = '\t';
	while (*msg)
		str[i++] = *(msg++);
	write(1, str, i);
	pthread_mutex_unlock(&philos->arguments->lock_status);
}

static inline void	fixed_usleep(unsigned int u_sec)
{
	struct timeval now;
	struct timeval step;

	gettimeofday(&now, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&step, NULL);
		if ((step.tv_sec - now.tv_sec) * 1000000 + (step.tv_usec - now.tv_usec) >= u_sec)
			return ;
	}
}

void	monitoring(t_philosopher *philos)
{
	int i;
	int j;
	long long time_since_last_eat;
	struct timeval now;

	i = 0;
	while (1)
	{
		if (i == philos->arguments->number_of_philosopher)
			i = 0;
		gettimeofday(&now, NULL);
		time_since_last_eat = (now.tv_sec - philos[i].last_meal->tv_sec) * 1000 + (now.tv_usec - philos[i].last_meal->tv_usec) * 0.001;
		if (time_since_last_eat > philos->arguments->time_to_die)
		{
			status_philo(philos, "died\n");
			pthread_mutex_lock(&philos->arguments->dead);
			g_philo_dead = 1;
			pthread_mutex_unlock(&philos->arguments->dead);
			j = 0;
			while (j < philos->arguments->number_of_philosopher)
			{
				pthread_join(*philos[j].philo, NULL);
				j++;
			}
			return ;
		}
		else if(philos->arguments->number_of_time_each_philosophers_must_eat != -1
			&& philos->arguments->nb_has_eaten >= (philos->arguments->number_of_philosopher * philos->arguments->number_of_time_each_philosophers_must_eat))
		{
			ft_putstr_fd("Everyone has at least eaten the amount of time expected\n", 1);
			pthread_mutex_lock(&philos->arguments->dead);
			g_philo_dead = 1;
			pthread_mutex_unlock(&philos->arguments->dead);
			j = 0;
			while (j < philos->arguments->number_of_philosopher)
			{
				pthread_join(*philos[j].philo, NULL);
				j++;
			}
			return ;
		}
		i++;
		fixed_usleep(1000);
	}
}

int		launch_philos(t_philosopher *philos)
{
	int i;

	i = 0;
	gettimeofday(&philos->arguments->start_philo, NULL);
	while (i < philos->arguments->number_of_philosopher)
	{
		philos[i].last_meal->tv_sec = philos->arguments->start_philo.tv_sec;
		philos[i].last_meal->tv_usec = philos->arguments->start_philo.tv_usec;
		if (pthread_create(philos[i].philo, NULL, &living, &(philos[i])) != 0)
			return (1);
		i++;
	}
	
	monitoring(philos);
	return (0);
}

int main(int ac, char **av)
{
	t_init args;
	t_philosopher *philos;

	philos = NULL;
	init_args(&args);
	printf("AC = %d\n", ac);
	if ((ac != 5 && ac != 6) || ft_all_numbers(av) == 1)
		return (ft_error(1, philos, args.number_of_philosopher));
	if (ft_parsing(ac, av, &args) != 0)
		return (ft_error(2, philos, args.number_of_philosopher));
	init_philos(&philos, &args);

	if (launch_philos(philos) == 1)
		return (ft_error(3, philos, args.number_of_philosopher));


	ft_free(args.number_of_philosopher, philos);
	return (0);
}