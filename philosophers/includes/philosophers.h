#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>

# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/time.h>

# include <signal.h>
# include <errno.h>

# include <pthread.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
# include <dirent.h>

extern int g_philo_dead;

typedef struct s_init
{
    int	number_of_philosopher;
    int	time_to_die;
    int	time_to_eat;
    int	time_to_sleep;
	int	number_of_time_each_philosophers_must_eat;
	int		nb_has_eaten;
	struct timeval start_philo;
	pthread_mutex_t dead;
	pthread_mutex_t lock_status;
}               t_init;

typedef struct s_philosopher
{
    pthread_t	*philo;
	char	numero_philo[20];
	pthread_mutex_t *fork_right;
	pthread_mutex_t *fork_left;
	int		num_philo;
	struct timeval *last_meal;
	t_init *arguments;
}               t_philosopher;

/* ERROR.C */

void	ft_free(int nb_of_phil, t_philosopher *philos);
int     ft_error(int error_code, t_philosopher *philos, int number_of_philosopher);

/* FT_DISPLAY.C */

void    ft_print_args(t_init *args);

/* FT_UTILS.C */

int		ft_isdigit(int c);
int		ft_atoi(const char *str);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
size_t		ft_strlen(const char *str);


/* INIT_VARIABLES.C */

int		print_nbr(size_t nbr, char *str);
int		init_philos(t_philosopher **philos, t_init *args);
void	init_args(t_init *args);

/* LIFE.C */

// void	status_philo(t_philosopher *philos, char *msg);
void	*living(void *arg);

/* MAIN.C */

void	monitoring(t_philosopher *philos);
int		launch_philos(t_philosopher *philos);
int		main(int ac, char **av);

/* PARSING.C */

int		ft_all_numbers(char **av);
int		ft_parsing(int ac, char **av, t_init *args);

#endif