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

static inline int			check_if_dead(t_philosopher *philo)
{
	sem_wait(philo->arguments->dead);
	if (g_philo_dead == 1)
	{
		sem_post(philo->arguments->dead);
		return (1);
	}
	sem_post(philo->arguments->dead);
	return (0);
}

static inline void			status_philo(t_philosopher *philos, char *msg)
{
	struct timeval	now;
	int				i;
	int				j;
	char			str[64];
	long long		number;

	sem_wait(philos->arguments->lock_status);
	gettimeofday(&now, NULL);
	number = ((now.tv_sec - philos->arguments->start_philo.tv_sec) * 1000
		+ (now.tv_usec - philos->arguments->start_philo.tv_usec) * 0.001);
	i = print_nbr(number, str);
	j = 0;
	str[i++] = '\t';
	while (philos->numero_philo[j])
		str[i++] = philos->numero_philo[j++];
	str[i++] = '\t';
	while (*msg)
		str[i++] = *(msg++);
	if (check_if_dead(philos) == 0 || (*(msg - 2) == 'd'))
		write(1, str, i);
	sem_post(philos->arguments->lock_status);
}

static inline void		fixed_usleep(unsigned int u_sec)
{
	struct timeval now;
	struct timeval step;

	gettimeofday(&now, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&step, NULL);
		if ((step.tv_sec - now.tv_sec)
			* 1000000 + (step.tv_usec - now.tv_usec) >= u_sec)
			return ;
	}
}

static inline void		*monitoring3(t_philosopher *philos)
{
	sem_wait(philos->arguments->dead);
	g_philo_dead = 1;
	sem_post(philos->arguments->dead);
	status_philo(philos, "died\n");
	sem_wait(philos->arguments->lock_status);
	return (NULL);
}

static inline void		*monitoring2(t_philosopher *philos)
{
	sem_wait(philos->arguments->lock_status);
	g_philo_eaten = 1;
	sem_post(philos->arguments->lock_status);
	return (NULL);
}

void					*monitoring(void *arg)
{
	long long		time_since_last_eat;
	struct timeval	now;
	t_philosopher	*philos;

	philos = arg;
	while (1)
	{
		gettimeofday(&now, NULL);
		time_since_last_eat = (now.tv_sec - philos->last_meal->tv_sec)
		* 1000 + (now.tv_usec - philos->last_meal->tv_usec) * 0.001;
		if (time_since_last_eat > philos->arguments->time_to_die)
			return (monitoring3(philos));
		else if (sem_wait(philos->arguments->lock_status) == 0
			&& philos->arguments->n_must_eat != -1
			&& philos->arguments->nb_has_eaten >=
			philos->arguments->n_must_eat
			&& sem_post(philos->arguments->lock_status) == 0)
			return (monitoring2(philos));
		sem_post(philos->arguments->lock_status);
		fixed_usleep(1000);
	}
	return (NULL);
}
