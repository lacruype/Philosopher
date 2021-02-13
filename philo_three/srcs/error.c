/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	ft_free(int number_of_philo, t_philosopher *philos)
{
	int i;

	i = 0;
	while (i < number_of_philo)
	{
		free(philos[i].philo);
		sem_close(philos[i].fork);
		sem_unlink("fork");
		free(philos[i].last_meal);
		i++;
	}
	sem_close(philos->arguments->dead);
	sem_unlink("dead");
	sem_close(philos->arguments->lock_status);
	sem_unlink("lock_status");
	sem_close(philos->arguments->perm_fork);
	sem_unlink("perm_fork");
	free(philos);
}

int		ft_error(int error_code, t_philosopher *philos, int number_of_philo)
{
	if (error_code == 1)
		printf("Incorrect number of arguments "
		"or alphabetic argument\nError Code %d\n", error_code);
	else if (error_code == 2)
		printf("Error Parsing\nError Code %d\n", error_code);
	else if (error_code == 3)
		printf("Thread error\nError Code %d\n", error_code);
	if (philos != NULL)
		ft_free(number_of_philo, philos);
	return (-1);
}
