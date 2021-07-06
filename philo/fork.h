#ifndef FORK_H
# define FORK_H
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>

typedef struct s_fork
{
	int				id;
	bool			used;
	pthread_mutex_t	mutex;
}	t_fork;

t_fork	*create_fork(int id);
t_fork	**create_forks(int amount);
void	destroy_forks(t_fork **forks, int amount);
void	lock_forks(t_fork *left_fork, t_fork *right_fork);
void	unlock_forks(t_fork *left_fork, t_fork *right_fork);
t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout);
t_fork	*get_left_fork(int id, t_fork **forks);
#endif
