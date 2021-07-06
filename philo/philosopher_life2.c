#include "philosopher_life.h"

bool	end_checker(t_philo *me)
{
	if (g_simulation.ended)
		return (true);
	if (g_simulation.options.nbr_of_times_each_philosopher_must_eat != -1
		&& g_simulation.satiated_philo
		== g_simulation.options.nbr_of_philosophers)
	{
		g_simulation.ended = true;
		return (true);
	}
	if (get_time_millis()
		- me->starving_since
		> g_simulation.options.time_to_die)
	{
		me->dead = true;
		g_simulation.ended = true;
		return (true);
	}
	return (false);
}

void	print_status(t_philo *me, char *msg)
{
	if (end_checker(me) && !me->dead)
		return ;
	pthread_mutex_lock(&g_simulation.general_mutex);
	printf("%llu %d %s\n", get_time_millis(), me->id, msg);
	pthread_mutex_unlock(&g_simulation.general_mutex);
}

bool	philosopher_sleep(t_philo *me, unsigned int millis)
{
	unsigned long long	start;

	start = get_time_millis();
	while (get_time_millis() - start < millis)
	{
		if (end_checker(me))
			return (true);
		usleep(1000);
	}
	return (false);
}

void	p_think(t_philo *me)
{
	if (end_checker(me))
		return ;
	print_status(me, "is thinking");
	usleep(1000);
}
