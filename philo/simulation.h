#ifndef SIMULATION_H
# define SIMULATION_H

# include "philosopher.h"

void	create_simulation(t_options options, t_fork **forks, t_philo **philos);
void	destroy_simulation(void);
void	add_satiated_philo(void);

t_simulation	g_simulation;

#endif
