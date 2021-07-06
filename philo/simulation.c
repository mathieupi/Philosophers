#include "simulation.h"

void	destroy_simulation(void)
{
	remove_philosophers(g_simulation.philosophers,
		g_simulation.options.nbr_of_philosophers);
	destroy_forks(g_simulation.forks, g_simulation.options.nbr_of_philosophers);
	pthread_mutex_destroy(&g_simulation.general_mutex);
}

void	start_simulation(t_options options)
{
	g_simulation.options = options;
	g_simulation.forks = create_forks(options.nbr_of_philosophers);
	g_simulation.philosophers
		= create_philosophers(options.nbr_of_philosophers);
	g_simulation.satiated_philo = 0;
	pthread_mutex_init(&g_simulation.general_mutex, NULL);
	start_philosophers(g_simulation.philosophers, options.nbr_of_philosophers);
	destroy_simulation();
}

void	add_satiated_philo(void)
{
	pthread_mutex_lock(&g_simulation.general_mutex);
	g_simulation.satiated_philo++;
	pthread_mutex_unlock(&g_simulation.general_mutex);
}
