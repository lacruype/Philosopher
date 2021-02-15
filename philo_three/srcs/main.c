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

int					main(int ac, char **av)
{
	t_init			args;
	t_philosopher	*philos;

	philos = NULL;
	init_args(&args);
	sem_unlink("dead");
	sem_unlink("lock_status");
	sem_unlink("fork");
	sem_unlink("perm_fork");
	if ((ac != 5 && ac != 6) || ft_all_numbers(av) == 1)
		return (ft_error(1, philos, args.number_of_philosopher));
	if (ft_parsing(ac, av, &args) != 0)
		return (ft_error(2, philos, args.number_of_philosopher));
	args.perm_fork = sem_open("perm_fork", O_CREAT,
		S_IRWXU, (args.number_of_philosopher / 2));
	init_philos(&philos, &args);
	if (launch_philos(philos) == 1)
		return (ft_error(3, philos, args.number_of_philosopher));
	ft_free(args.number_of_philosopher, philos);
	return (0);
}
