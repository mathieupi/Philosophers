#ifndef SIMULATION_H
# define SIMULATION_H
# include "philosopher.h"
# include "fork.h"

typedef struct s_options
{
	unsigned int	nbr_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nbr_of_times_each_philosopher_must_eat;
}	t_options;

typedef struct s_simulation
{
	t_options		options;
	t_philo			**philosophers;
	t_fork			**forks;
	unsigned int	satiated_philo;
	bool			ended;
	pthread_mutex_t	general_mutex;
}	t_simulation;

void	start_simulation(t_options options);
void	add_satiated_philo(void);

t_simulation	g_simulation;

#endif
