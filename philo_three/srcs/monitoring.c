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
	return (NULL);
}

static inline void		*monitoring2(t_philosopher *philos)
{
	sem_wait(philos->arguments->lock_status);
	g_philo_eaten = 1;
	sem_post(philos->arguments->lock_status);
	// ft_putstr_fd("Everyone has eaten the amount of time expected\n", 1);
	return (NULL);
}

void					*monitoring(void *arg)
{
	long long		time_since_last_eat;
	struct timeval	now;
	t_philosopher *philos;

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
