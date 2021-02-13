/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static inline void		status_philo(t_philosopher *philos, char *msg)
{
	struct timeval	now;
	int				i;
	int				j;
	char			str[64];
	long long		number;

	pthread_mutex_lock(&philos->arguments->lock_status);
	gettimeofday(&now, NULL);
	number = ((now.tv_sec - philos->arguments->start_philo.tv_sec)
	* 1000 + (now.tv_usec - philos->arguments->start_philo.tv_usec) * 0.001);
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

static inline void		monitoring3(t_philosopher *philos)
{
	int j;

	j = 0;
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

static inline void		monitoring2(t_philosopher *philos)
{
	int j;

	j = 0;
	pthread_mutex_lock(&philos->arguments->dead);
	g_philo_dead = 1;
	pthread_mutex_unlock(&philos->arguments->dead);
	ft_putstr_fd("Everyone has eaten the amount of time expected\n", 1);
	while (j < philos->arguments->number_of_philosopher)
	{
		pthread_join(*philos[j].philo, NULL);
		j++;
	}
	return ;
}

void					monitoring(t_philosopher *philos)
{
	int				i;
	long long		time_since_last_eat;
	struct timeval	now;

	i = 0;
	while (1)
	{
		if (i == philos->arguments->number_of_philosopher)
			i = 0;
		gettimeofday(&now, NULL);
		time_since_last_eat = (now.tv_sec - philos[i].last_meal->tv_sec)
		* 1000 + (now.tv_usec - philos[i].last_meal->tv_usec) * 0.001;
		if (time_since_last_eat > philos->arguments->time_to_die)
			return (monitoring3(philos));
		else if (pthread_mutex_lock(&philos->arguments->lock_status) == 0
			&& philos->arguments->n_must_eat != -1
			&& philos->arguments->nb_has_eaten >=
			(philos->arguments->number_of_philosopher
			* philos->arguments->n_must_eat)
			&& pthread_mutex_unlock(&philos->arguments->lock_status) == 0)
			return (monitoring2(philos));
		pthread_mutex_unlock(&philos->arguments->lock_status);
		i++;
		fixed_usleep(1000);
	}
}
