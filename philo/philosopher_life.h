#ifndef PHILOSOPHER_LIFE_H
# define PHILOSOPHER_LIFE_H
# include "philosopher_types.h"
# include "simulation.h"

void	*routine(void *arg);
t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout);
t_fork	*get_left_fork(int id, t_fork **forks);
bool	end_checker(t_args *args);
void	print_status(t_args *args, char *msg);
bool	philosopher_sleep(t_args *args, unsigned int millis);
void	p_think(t_args *args);
void	try_getting_fork(t_args *args);
void	p_eat(t_args *args);
void	p_sleep(t_args *args);
#endif
