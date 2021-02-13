/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int g_philo_dead = 0;

static inline void			fixed_usleep(unsigned int u_sec)
{
	struct timeval now;
	struct timeval step;

	gettimeofday(&now, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&step, NULL);
		if ((step.tv_sec - now.tv_sec) * 1000000
			+ (step.tv_usec - now.tv_usec) >= u_sec)
			return ;
	}
}

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
	if (check_if_dead(philos) == 0)
		write(1, str, i);
	sem_post(philos->arguments->lock_status);
}

static inline int			ft_eating_philo(t_philosopher *philos)
{
	sem_wait(philos->arguments->perm_fork);
	sem_wait(philos->fork);
	if (check_if_dead(philos) == 1)
	{
		sem_post(philos->arguments->perm_fork);
		return (sem_post(philos->fork) + 1);
	}
	status_philo(philos, "has taken a fork\n");
	sem_wait(philos->fork);
	if (check_if_dead(philos) == 1)
	{
		sem_post(philos->arguments->perm_fork);
		sem_post(philos->fork);
		return (sem_post(philos->fork) + 1);
	}
	status_philo(philos, "has taken a fork\n");
	status_philo(philos, "is eating\n");
	gettimeofday(philos->last_meal, NULL);
	sem_wait(philos->arguments->lock_status);
	philos->arguments->nb_has_eaten++;
	sem_post(philos->arguments->lock_status);
	fixed_usleep(philos->arguments->time_to_eat * 1000);
	sem_post(philos->fork);
	sem_post(philos->fork);
	return (sem_post(philos->arguments->perm_fork));
}

/*
**	X1000 because Miliseconds -> Microseconds
*/

int						living(void *arg)
{
	t_philosopher *philo;

	philo = arg;
	while (1)
	{
		if (check_if_dead(philo) == 1)
			return (1);
		if (ft_eating_philo(arg) == 1)
			return (1);
		sem_wait(philo->arguments->lock_status);
		if (g_philo_eaten == 1)
			return (sem_post(philo->arguments->lock_status) + 2);
		sem_post(philo->arguments->lock_status);
		if (check_if_dead(philo) == 1)
			return (1);
		status_philo(arg, "is sleeping\n");
		if (check_if_dead(philo) == 1)
			return (1);
		fixed_usleep(philo->arguments->time_to_sleep * 1000);
		if (check_if_dead(philo) == 1)
			return (1);
		status_philo(arg, "is thinking\n");
	}
	return (0);
}
