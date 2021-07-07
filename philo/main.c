/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:27:57 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:40 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "philosopher.h"

t_options	parse_options(int ac, char **av)
{
	t_options	options;

	options.nbr_of_philosophers = parse_int(av[0]);
	options.time_to_die = parse_int(av[1]);
	options.time_to_eat = parse_int(av[2]);
	options.time_to_sleep = parse_int(av[3]);
	if (ac == 5)
		options.nbr_of_times_each_philosopher_must_eat = parse_int(av[4]);
	else
		options.nbr_of_times_each_philosopher_must_eat = -1;
	return (options);
}

bool	is_valid_arg(char *arg)
{
	if (!arg)
		return (false);
	while (*arg)
	{
		if (!is_digit(*arg))
			return (false);
		arg++;
	}
	return (true);
}

bool	is_valid_args(int ac, char **av)
{
	int	i;

	if (ac < 4 || ac > 5)
	{
		printf("Bad args :/ I need\n"
			"./philo number_of_philosophers time_to_die time_to_eat"
			"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (false);
	}
	i = 0;
	while (i < ac)
	{
		if (!is_valid_arg(av[i]))
		{
			printf("Invalid args :/\n");
			return (false);
		}
		i++;
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_options	options;

	ac--;
	av++;
	if (!is_valid_args(ac, av))
		return (0);
	options = parse_options(ac, av);
	if (options.nbr_of_times_each_philosopher_must_eat == 0)
		return (0);
	start_simulation(options);
}
