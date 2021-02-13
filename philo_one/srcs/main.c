/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int					launch_philos(t_philosopher *philos)
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

int					main(int ac, char **av)
{
	t_init			args;
	t_philosopher	*philos;

	philos = NULL;
	init_args(&args);
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
