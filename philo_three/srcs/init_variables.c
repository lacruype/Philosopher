/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_variables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int		init_philos(t_philosopher **phil, t_init *a)
{
	int				i;
	t_philosopher	*philos;

	i = -1;
	philos = *phil;
	if (!(philos = malloc(sizeof(t_philosopher) * (a->number_of_philosopher))))
		return (1);
	while (++i < a->number_of_philosopher)
	{
		philos[i].arguments = a;
		philos[i].num_philo = (i + 1);
		philos[i].has_finish_eaten = 0;
		philos[i].arguments->nb_has_eaten = 0;
		ft_bzero(philos[i].numero_philo, 20);
		print_nbr((i + 1), philos[i].numero_philo);
		if (!(philos[i].philo = malloc(sizeof(pid_t))))
			return (1);
		philos[i].fork = sem_open("fork", O_CREAT,
			S_IRWXU, philos->arguments->number_of_philosopher);
		if (!(philos[i].last_meal = malloc(sizeof(struct timeval))))
			return (1);
	}
	return ((*phil = philos) ? 0 : 1);
}

void	init_args(t_init *args)
{
	g_philo_eaten = 0;
	g_philo_dead = 0;
	args->number_of_philosopher = 0;
	args->time_to_die = 0;
	args->time_to_eat = 0;
	args->time_to_sleep = 0;
	args->n_must_eat = -1;
	args->dead = sem_open("dead", O_CREAT, S_IRWXU, 1);
	args->lock_status = sem_open("lock_status", O_CREAT, S_IRWXU, 1);
}
