#ifndef SIMULATION_H
# define SIMULATION_H
# include "philosopher_types.h"
# include "philosopher.h"
# include "fork.h"

void	start_simulation(t_options options);
void	add_satiated_philo(t_simulation *simulation);

#endif
