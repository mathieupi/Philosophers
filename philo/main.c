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

int	main(int ac, char **av)
{
	t_options	options;
	t_fork		**fork_list;
	t_philo		**philo_list;

	ac--;
	av++;
	if (ac < 4)
	{
		printf("not enough args\n");
		return (0);
	}
	if (ac > 5)
	{
		printf("too much args\n");
		return (0);
	}
	options = parse_options(ac, av);
	if (options.nbr_of_times_each_philosopher_must_eat == 0)
		return (0);
	fork_list = create_forks(options.nbr_of_philosophers);
	philo_list = create_philosophers(options.nbr_of_philosophers);
	create_simulation(options, fork_list, philo_list);
	start_philosophers(philo_list, options.nbr_of_philosophers);
	remove_philosophers(philo_list, options.nbr_of_philosophers);
	destroy_forks(fork_list, options.nbr_of_philosophers);
	destroy_simulation();
}
