#include "../includes/philosophers.h"

void	ft_free(int number_of_philo, t_philosopher *philos)
{
	int i;

	i = 0;
	while (i < number_of_philo)
	{
		free(philos[i].philo);
		pthread_mutex_destroy(philos[i].fork_right);
		free(philos[i].fork_right);
		free(philos[i].last_meal);
		i++;
	}
	pthread_mutex_destroy(&philos->arguments->dead);
	pthread_mutex_destroy(&philos->arguments->lock_status);
	free(philos);
}

int     ft_error(int error_code, t_philosopher *philos, int number_of_philo)
{
    if (error_code == 1)
        printf("Incorrect number of argument or alphabetic argument\nError Code %d\n", error_code);
    else if (error_code == 2)
		printf("Error Parsing\nError Code %d\n", error_code);
	else if (error_code == 3)
		printf("Thread error\nError Code %d\n", error_code);
	if (philos != NULL)
		ft_free(number_of_philo, philos);
    return (-1);
}