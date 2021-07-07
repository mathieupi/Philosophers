#include "philosopher_life.h"

bool	end_checker(t_args *args)
{
	if (args->simulation->ended)
		return (true);
	if (args->simulation->options.nbr_of_times_each_philosopher_must_eat != -1
		&& args->simulation->satiated_philo
		== args->simulation->options.nbr_of_philosophers)
	{
		args->simulation->ended = true;
		return (true);
	}
	if (get_time_millis()
		- args->me->starving_since
		> args->simulation->options.time_to_die)
	{
		args->me->dead = true;
		args->simulation->ended = true;
		return (true);
	}
	return (false);
}

void	print_status(t_args *args, char *msg)
{
	if (end_checker(args) && !args->me->dead)
		return ;
	pthread_mutex_lock(&args->simulation->general_mutex);
	printf("%llu %d %s\n", get_time_millis(), args->me->id, msg);
	pthread_mutex_unlock(&args->simulation->general_mutex);
}

bool	philosopher_sleep(t_args *args, unsigned int millis)
{
	unsigned long long	start;

	start = get_time_millis();
	while (get_time_millis() - start < millis)
	{
		if (end_checker(args))
			return (true);
		usleep(1000);
	}
	return (false);
}

void	p_think(t_args *args)
{
	if (end_checker(args))
		return ;
	print_status(args, "is thinking");
	usleep(1000);
}
