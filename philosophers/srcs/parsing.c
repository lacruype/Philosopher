#include "../includes/philosophers.h"

int		ft_all_numbers(char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int    ft_parsing(int ac, char **av, t_init *args)
{
	args->number_of_philosopher = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		args->number_of_time_each_philosophers_must_eat = ft_atoi(av[5]);
	if (args->number_of_philosopher < 0
		|| args->time_to_die < 0 || args->time_to_eat < 0 || args->time_to_sleep < 0)
		return (1);
	if (ac == 6 && args->number_of_time_each_philosophers_must_eat < 0)
		return (1);	
	return (0);
}