#include "philosopher.h"

t_philo	*create_philosopher(int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->thread = 0;
	philo->id = id;
	philo->fork_in_hand = 0;
	philo->eating = false;
	philo->sleeping = false;
	philo->thinking = false;
	philo->dead = false;
	philo->starving_since = get_time_millis();
	philo->eat_counter = 0;
	return (philo);
}

t_philo	**create_philosophers(int amount)
{
	t_philo	**philo_list;
	int		i;

	i = 0;
	philo_list = malloc(sizeof(t_philo *) * amount);
	while (i < amount)
	{
		philo_list[i] = create_philosopher(i + 1);
		i++;
	}
	return (philo_list);
}

void	start_philosophers(t_simulation *simulation)
{
	unsigned int	i;
	t_args			*args;

	i = 0;
	while (i < simulation->options.nbr_of_philosophers)
	{
		args = malloc(sizeof(t_args));
		args->me = simulation->philosophers[i];
		args->simulation = simulation;
		pthread_create(&simulation->philosophers[i]->thread,
			NULL, routine, args);
		i++;
	}
}

void	remove_philosophers(t_philo **philo_list, int amout)
{
	int	i;

	i = 0;
	while (i < amout)
	{
		pthread_join(philo_list[i]->thread, NULL);
		free(philo_list[i]);
		i++;
	}
	free(philo_list);
}
