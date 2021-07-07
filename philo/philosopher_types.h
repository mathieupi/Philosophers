#ifndef PHILOSOPHER_TYPES_H
# define PHILOSOPHER_TYPES_H
# include <stdbool.h>
# include <pthread.h>

typedef struct s_fork
{
	int				id;
	bool			used;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_options
{
	unsigned int	nbr_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nbr_of_times_each_philosopher_must_eat;
}	t_options;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					fork_in_hand;
	bool				eating;
	bool				sleeping;
	bool				thinking;
	bool				dead;
	unsigned long long	starving_since;
	int					eat_counter;
}	t_philo;

typedef struct s_simulation
{
	t_options		options;
	t_philo			**philosophers;
	t_fork			**forks;
	unsigned int	satiated_philo;
	bool			ended;
	pthread_mutex_t	general_mutex;
}	t_simulation;

typedef struct s_args
{
	t_simulation	*simulation;
	t_philo			*me;
}	t_args;
#endif
