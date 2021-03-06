/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:28:19 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:35 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

void	destroy_simulation(t_simulation *simulation)
{
	remove_philosophers(simulation->philosophers,
		simulation->options.nbr_of_philosophers);
	destroy_forks(simulation->forks, simulation->options.nbr_of_philosophers);
	pthread_mutex_destroy(&simulation->general_mutex);
	free(simulation);
}

static t_simulation	*create_simulation(t_options options)
{
	t_simulation	*simulation;

	simulation = malloc(sizeof(t_simulation));
	simulation->options = options;
	simulation->forks = create_forks(options.nbr_of_philosophers);
	simulation->philosophers
		= create_philosophers(options.nbr_of_philosophers);
	simulation->satiated_philo = 0;
	simulation->ended = false;
	pthread_mutex_init(&simulation->general_mutex, NULL);
	return (simulation);
}

void	start_simulation(t_options options)
{
	t_simulation	*simulation;

	simulation = create_simulation(options);
	start_philosophers(simulation);
	destroy_simulation(simulation);
}

void	add_satiated_philo(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->general_mutex);
	simulation->satiated_philo++;
	pthread_mutex_unlock(&simulation->general_mutex);
}
