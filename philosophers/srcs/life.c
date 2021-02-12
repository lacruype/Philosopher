#include "../includes/philosophers.h"

int g_philo_dead = 0;

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

static inline int		check_if_dead(t_philosopher *philo)
{
	(void)philo;
	pthread_mutex_lock(&philo->arguments->dead);
	if (g_philo_dead == 1)
	{
		pthread_mutex_unlock(&philo->arguments->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->arguments->dead);
	return (0);
}

static inline int	ft_eating_philo(t_philosopher *philos)
{
	if	((philos->num_philo % 2) == 1)
		pthread_mutex_lock(philos->fork_right);
	else
		pthread_mutex_lock(philos->fork_left);
	status_philo(philos, "has taken a fork\n");
	if	((philos->num_philo % 2) == 0)
		pthread_mutex_lock(philos->fork_right);
	else
		pthread_mutex_lock(philos->fork_left);
	if (check_if_dead(philos) == 1)
			return (1);
	status_philo(philos, "has taken a fork\n");
	if (check_if_dead(philos) == 1)
			return (1);
	status_philo(philos, "is eating\n");
	gettimeofday(philos->last_meal, NULL);
	philos->arguments->nb_has_eaten++;
	fixed_usleep(philos->arguments->time_to_eat * 1000);
	pthread_mutex_unlock(philos->fork_right);
	pthread_mutex_unlock(philos->fork_left);
	return (0);
}

/* X1000 because Miliseconds -> Microseconds*/

void	*living(void *arg)
{
	t_philosopher *philo;

	philo = arg;
	while (1)
	{
		if (check_if_dead(philo) == 1)
			return (NULL);
		if (ft_eating_philo(arg) == 1)
			return (NULL);
		if (check_if_dead(philo) == 1)
			return (NULL);
		status_philo(arg, "is sleeping\n");
		if (check_if_dead(philo) == 1)
			return (NULL);
		fixed_usleep(philo->arguments->time_to_sleep * 1000);
		if (check_if_dead(philo) == 1)
			return (NULL);
		status_philo(arg, "is thinking\n");
	}
	return (NULL);
}