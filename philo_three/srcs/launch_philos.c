/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static inline int			check_if_everyone_eat(t_philosopher *philos)
{
	int i;

	i = 0;
	while (i < philos->arguments->number_of_philosopher
		&& philos[i].has_finish_eaten == 2)
		i++;
	if (i == philos->arguments->number_of_philosopher)
		return (1);
	return (0);
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

void						launch_philos2(t_philosopher *philos,
	int i, int j, int status)
{
	while (1)
	{
		waitpid(*philos[++i].philo, &status, WNOHANG | WUNTRACED);
		if (WIFEXITED(status))
			if ((philos[i].has_finish_eaten = WEXITSTATUS(status)))
			{
				if (check_if_everyone_eat(philos) == 1)
				{
					sem_wait(philos->arguments->lock_status);
					ft_putstr_fd("Everyone has eaten the enough\n", 1);
					sem_post(philos->arguments->lock_status);
				}
				else if (philos[i].has_finish_eaten == 1)
				{
					status_philo(&philos[i], "died\n");
					while (++j < philos->arguments->number_of_philosopher)
						kill(*philos[j].philo, SIGKILL);
				}
				if (philos[i].has_finish_eaten == 1
					|| check_if_everyone_eat(philos) == 1)
					return (0);
			}
		if (i >= philos->arguments->number_of_philosopher)
			i = -1;
	}
}

int							launch_philos(t_philosopher *philos)
{
	int i;
	int j;
	int status;

	i = 0;
	j = -1;
	status = 0;
	gettimeofday(&philos->arguments->start_philo, NULL);
	while (i < philos->arguments->number_of_philosopher)
	{
		philos[i].last_meal->tv_sec = philos->arguments->start_philo.tv_sec;
		philos[i].last_meal->tv_usec = philos->arguments->start_philo.tv_usec;
		if ((*philos[i].philo = fork()) == 0)
		{
			pthread_create(&philos[i].monitor, NULL, &monitoring, &(philos[i]));
			pthread_detach(philos[i].monitor);
			exit(living(&philos[i]));
		}
		i++;
	}
	i = -1;
	launch_philos2(philos, i, j, status);
	return (0);
}
