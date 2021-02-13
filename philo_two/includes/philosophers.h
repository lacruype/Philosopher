/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lacruype <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:22:36 by lacruype          #+#    #+#             */
/*   Updated: 2021/02/12 15:34:52 by lacruype         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>

# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/time.h>

# include <signal.h>
# include <errno.h>

# include <semaphore.h>
# include <pthread.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
# include <dirent.h>

extern	int			g_philo_dead;

typedef	struct		s_init
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_must_eat;
	int				nb_has_eaten;
	struct timeval	start_philo;
	sem_t			*dead;
	sem_t			*lock_status;
	sem_t			*perm_fork;
}					t_init;

typedef	struct		s_philosopher
{
	pthread_t		*philo;
	char			numero_philo[20];
	sem_t			*fork;
	int				num_philo;
	struct timeval	*last_meal;
	t_init			*arguments;
}					t_philosopher;

/*
**	ERROR.C
*/

void				ft_free(int nb_of_phil, t_philosopher *philos);
int					ft_error(int error_code, t_philosopher *philos, int n);

/*
**	FT_UTILS.C
*/

int					ft_isdigit(int c);
int					ft_atoi(const char *str);
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);

/*
**	FT_UTILS2.C
*/

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
size_t				ft_strlen(const char *str);

/*
**	INIT_VARIABLES.C
*/

int					print_nbr(size_t nbr, char *str);
int					init_philos(t_philosopher **philos, t_init *args);
void				init_args(t_init *args);

/*
**	LIFE.C
*/

void				*living(void *arg);

/*
**	MAIN.C
*/

void				monitoring(t_philosopher *philos);
int					launch_philos(t_philosopher *philos);
int					main(int ac, char **av);

/*
**	PARSING.C
*/

int					ft_all_numbers(char **av);
int					ft_parsing(int ac, char **av, t_init *args);

#endif
