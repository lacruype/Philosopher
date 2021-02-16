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

static inline void			launch_philos2(t_philosopher *philos,
	int i, int j, int status)
{
	while (1)
	{
		waitpid(*philos[i].philo, &status, WNOHANG | WUNTRACED);
		if (WIFEXITED(status))
			if ((philos[i].has_finish_eaten = WEXITSTATUS(status)))
			{
				if (philos[i].has_finish_eaten == 1)
				{
					while (++j < philos->arguments->number_of_philosopher)
						kill(*philos[j].philo, SIGKILL);
				}
				if (philos[i].has_finish_eaten == 1
					|| check_if_everyone_eat(philos) == 1)
					return ;
			}
		if (++i >= philos->arguments->number_of_philosopher)
			i = 0;
		fixed_usleep(1000);
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
	i = 0;
	launch_philos2(philos, i, j, status);
	return (0);
}
