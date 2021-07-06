#include "fork.h"

t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout)
{
	return (forks[(id + 1) % fork_amout]);
}

t_fork	*get_left_fork(int id, t_fork **forks)
{
	return (forks[id]);
}
