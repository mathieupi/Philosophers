#include "simulation.h"

void	create_simulation(t_options options, t_fork **forks, t_philo **philos)
{
	g_simulation.options = options;
	g_simulation.forks = forks;
	g_simulation.philosophers = philos;
	g_simulation.satiated_philo = 0;
	pthread_mutex_init(&g_simulation.general_mutex, NULL);
}

void	destroy_simulation(void)
{
	pthread_mutex_destroy(&g_simulation.general_mutex);
}

void	add_satiated_philo(void)
{
	pthread_mutex_lock(&g_simulation.general_mutex);
	g_simulation.satiated_philo++;
	pthread_mutex_unlock(&g_simulation.general_mutex);
}
