/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_life2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:27:37 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:37 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_life.h"

bool	end_checker(t_args *args)
{
	bool	result;

	pthread_mutex_lock(&args->simulation->general_mutex);
	result = args->simulation->ended;
	if (args->simulation->options.nbr_of_times_each_philosopher_must_eat != -1
		&& args->simulation->satiated_philo
		== args->simulation->options.nbr_of_philosophers)
	{
		args->simulation->ended = true;
		result |= true;
	}
	if (get_time_millis()
		- args->me->starving_since
		> args->simulation->options.time_to_die)
	{
		args->me->dead = true;
		args->simulation->ended = true;
		result |= true;
	}
	pthread_mutex_unlock(&args->simulation->general_mutex);
	return (result);
}

void	print_status(t_args *args, char *msg)
{
	if (!args->me->dead && end_checker(args))
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
		usleep(100);
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
