#ifndef FORK_H
# define FORK_H
# include "philosopher_types.h"
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>

t_fork	*create_fork(int id);
t_fork	**create_forks(int amount);
void	destroy_forks(t_fork **forks, int amount);
void	lock_forks(t_fork *left_fork, t_fork *right_fork);
void	unlock_forks(t_fork *left_fork, t_fork *right_fork);
t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout);
t_fork	*get_left_fork(int id, t_fork **forks);
#endif
