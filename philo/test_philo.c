#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_options
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_options;

typedef struct s_philo
{
	pthread_t	thread;
	int		id;
	int		fork_in_hand;
	bool	eating;
	bool	sleeping;
	bool	thinking;
	bool	dead;
	int		death_timer;
	int		sleep_timer;
	int		eat_timer;
}	t_philo;

typedef struct s_fork
{
	int				id;
	int				used;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_simulation
{
	t_options	options;
	t_philo		**philosophers;
	t_fork		**forks;
}	t_simulation;

t_simulation	g_simulation;

int	parse_int(const char *str)
{
	int	result;

	result = 0;
	while (str && *str >= '0' && *str <= '9')
		result = result * 10 + (*(str++) - '0');
	return (result);
}

t_options	parse_options(int ac, char **av)
{
	t_options	options;

	options.number_of_philosophers = parse_int(av[0]);
	options.time_to_die = parse_int(av[1]);
	options.time_to_eat = parse_int(av[2]);
	options.time_to_sleep = parse_int(av[3]);
	if (ac == 5)
		options.number_of_times_each_philosopher_must_eat = parse_int(av[4]);
	else
		options.number_of_times_each_philosopher_must_eat = -1;
	return (options);
}

unsigned long long	get_time_millis(void)
{
	struct timeval	tval;

	gettimeofday(&tval, NULL);
	return (tval.tv_sec * 1000 + tval.tv_usec / 1000);
}

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
	philo->death_timer = 0;
	philo->sleep_timer = 0;
	philo->eat_timer = 0;
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

t_fork	*create_fork(int id)
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	fork->id = id;
	fork->used = 0;
	pthread_mutex_init(&fork->mutex, NULL);
	return (fork);
}

void	destroy_forks(t_fork **forks, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		pthread_mutex_destroy(&(forks[i]->mutex));
		free(forks[i]);
		i++;
	}
	free(forks);
}

t_fork	**create_forks(int amount)
{
	t_fork	**fork_list;
	int		i;

	i = 0;
	fork_list = malloc(sizeof(t_fork *) * amount);
	while (i < amount)
	{
		fork_list[i] = create_fork(i + 1);
		i++;
	}
	return (fork_list);
}

t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout)
{
	return (forks[(id + 1) % fork_amout]);
}

t_fork	*get_left_fork(int id, t_fork **forks, int fork_amout)
{
	return (forks[id]);
}

void	lock_forks(t_fork *left_fork, t_fork *right_fork)
{
	if (right_fork->id > left_fork->id)
	{
		pthread_mutex_lock(&left_fork->mutex);
		pthread_mutex_lock(&right_fork->mutex);
	}
	else
	{
		pthread_mutex_lock(&right_fork->mutex);
		pthread_mutex_lock(&left_fork->mutex);
	}
}

void	unlock_forks(t_fork *left_fork, t_fork *right_fork)
{
	if (right_fork->id > left_fork->id)
	{
		pthread_mutex_unlock(&left_fork->mutex);
		pthread_mutex_unlock(&right_fork->mutex);
	}
	else
	{
		pthread_mutex_unlock(&right_fork->mutex);
		pthread_mutex_unlock(&left_fork->mutex);
	}
}

int p_eat(t_philo *me)
{
	int	result;

	result = 0;
	t_fork *right_fork = get_right_fork(me->id - 1, g_simulation.forks, g_simulation.options.number_of_philosophers);
	t_fork *left_fork = get_left_fork(me->id - 1, g_simulation.forks, g_simulation.options.number_of_philosophers);
	lock_forks(left_fork, right_fork);
	if (!right_fork->used && !left_fork->used)
	{
		printf("%llu %d is eating\n", get_time_millis(), me->id);
		right_fork->used = 1;
		left_fork->used = 1;
		me->fork_in_hand = 1;
		me->death_timer = get_time_millis();
		result = 1;
	}
	unlock_forks(left_fork, right_fork);
	return (result);
}

int	p_sleep(t_philo *me)
{
	printf("%llu %d is sleeping\n", get_time_millis(), me->id);
	usleep(g_simulation.options.time_to_sleep * 1000);
}

int	p_think(t_philo *me)
{
	printf("%llu %d is thinking\n", get_time_millis(), me->id);
}

void	*routine(void *arg)
{
	t_philo *me = (t_philo *)arg;
	printf("Hi I am philosopher %d :)\n", me->id);
	while (1)
	{
		if (p_eat(me))
		{
			usleep(g_simulation.options.time_to_eat * 1000);
			t_fork *right_fork = get_right_fork(me->id - 1, g_simulation.forks, g_simulation.options.number_of_philosophers);
			t_fork *left_fork = get_left_fork(me->id - 1, g_simulation.forks, g_simulation.options.number_of_philosophers);
			lock_forks(left_fork, right_fork);
			right_fork->used = 0;
			left_fork->used = 0;
			me->fork_in_hand = 0;
			unlock_forks(left_fork, right_fork);
		}
		else
			continue ;
		p_sleep(me);
		p_think(me);
	}
}

void	start_philosophers(t_philo **philo_list, int amout)
{
	int	i;

	i = 0;
	while (i < amout)
	{
		pthread_create(&philo_list[i]->thread, NULL, routine, philo_list[i]);
		i++;
	}
}

void	join_philosophers(t_philo **philo_list, int amout)
{
	int	i;

	i = 0;
	while (i < amout)
	{
		pthread_join(philo_list[i]->thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_options	options;

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
	printf("%d\n", options.number_of_philosophers);
	t_fork **fork_list = create_forks(options.number_of_philosophers);
	t_philo **philo_list = create_philosophers(options.number_of_philosophers);
	g_simulation.options = options;
	g_simulation.forks = fork_list;
	start_philosophers(philo_list, options.number_of_philosophers);
	join_philosophers(philo_list, options.number_of_philosophers);
	destroy_forks(fork_list, options.number_of_philosophers);
}
