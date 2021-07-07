#include "philosopher_life.h"
#include "fork.h"

void	try_getting_fork(t_args *args)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(args))
		return ;
	right_fork = get_right_fork(args->me->id - 1, args->simulation->forks,
			args->simulation->options.nbr_of_philosophers);
	left_fork = get_left_fork(args->me->id - 1, args->simulation->forks);
	if (left_fork == right_fork)
		return ;
	lock_forks(left_fork, right_fork);
	if (!right_fork->used && !left_fork->used)
	{
		print_status(args, "has taken a fork");
		print_status(args, "has taken a fork");
		right_fork->used = true;
		left_fork->used = true;
		args->me->fork_in_hand = 2;
	}
	unlock_forks(left_fork, right_fork);
}

void	p_eat(t_args *args)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(args))
		return ;
	right_fork = get_right_fork(args->me->id - 1, args->simulation->forks,
			args->simulation->options.nbr_of_philosophers);
	left_fork = get_left_fork(args->me->id - 1, args->simulation->forks);
	args->me->starving_since = get_time_millis();
	print_status(args, "is eating");
	args->me->eat_counter++;
	if (args->me->eat_counter
		== args->simulation->options.nbr_of_times_each_philosopher_must_eat)
		add_satiated_philo(args->simulation);
	if (philosopher_sleep(args, args->simulation->options.time_to_eat))
		return ;
	lock_forks(left_fork, right_fork);
	right_fork->used = false;
	left_fork->used = false;
	unlock_forks(left_fork, right_fork);
	args->me->fork_in_hand = 0;
}

void	p_sleep(t_args *args)
{
	if (end_checker(args))
		return ;
	print_status(args, "is sleeping");
	philosopher_sleep(args, args->simulation->options.time_to_sleep);
}

void	*routine(void *arg)
{
	t_args	*args;

	args = (t_args *)arg;
	while (1)
	{
		usleep(100);
		end_checker(args);
		if (args->me->dead)
		{
			print_status(args, "died");
			break ;
		}
		if (args->simulation->ended)
			break ;
		try_getting_fork(args);
		if (args->me->fork_in_hand != 2)
			continue ;
		p_eat(args);
		p_sleep(args);
		p_think(args);
	}
	return (NULL);
}
