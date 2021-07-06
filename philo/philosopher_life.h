#ifndef PHILOSOPHER_LIFE_H
# define PHILOSOPHER_LIFE_H
# include "simulation.h"

void	*routine(void *arg);
t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout);
t_fork	*get_left_fork(int id, t_fork **forks);
bool	end_checker(t_philo *me);
void	print_status(t_philo *me, char *msg);
bool	philosopher_sleep(t_philo *me, unsigned int millis);
void	p_think(t_philo *me);
void	try_getting_fork(t_philo *me);
void	p_eat(t_philo *me);
void	p_sleep(t_philo *me);
#endif
