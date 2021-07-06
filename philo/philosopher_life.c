#include "philosopher_life.h"
#include "fork.h"

void	try_getting_fork(t_philo *me)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(me))
		return ;
	right_fork = get_right_fork(me->id - 1, g_simulation.forks,
			g_simulation.options.nbr_of_philosophers);
	left_fork = get_left_fork(me->id - 1, g_simulation.forks);
	if (left_fork == right_fork)
		return ;
	lock_forks(left_fork, right_fork);
	if (!right_fork->used && !left_fork->used)
	{
		print_status(me, "has taken a fork");
		print_status(me, "has taken a fork");
		right_fork->used = true;
		left_fork->used = true;
		me->fork_in_hand = 2;
	}
	unlock_forks(left_fork, right_fork);
}

void	p_eat(t_philo *me)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(me))
		return ;
	right_fork = get_right_fork(me->id - 1, g_simulation.forks,
			g_simulation.options.nbr_of_philosophers);
	left_fork = get_left_fork(me->id - 1, g_simulation.forks);
	me->starving_since = get_time_millis();
	print_status(me, "is eating");
	me->eat_counter++;
	if (me->eat_counter
		== g_simulation.options.nbr_of_times_each_philosopher_must_eat)
		add_satiated_philo();
	if (philosopher_sleep(me, g_simulation.options.time_to_eat))
		return ;
	lock_forks(left_fork, right_fork);
	right_fork->used = false;
	left_fork->used = false;
	unlock_forks(left_fork, right_fork);
	me->fork_in_hand = 0;
}

void	p_sleep(t_philo *me)
{
	if (end_checker(me))
		return ;
	print_status(me, "is sleeping");
	philosopher_sleep(me, g_simulation.options.time_to_sleep);
}

void	*routine(void *arg)
{
	t_philo	*me;

	me = (t_philo *)arg;
	while (1)
	{
		usleep(100);
		end_checker(me);
		if (me->dead)
		{
			print_status(me, "died");
			break ;
		}
		if (g_simulation.ended)
			break ;
		try_getting_fork(me);
		if (me->fork_in_hand != 2)
			continue ;
		p_eat(me);
		p_sleep(me);
		p_think(me);
	}
	return (NULL);
}
