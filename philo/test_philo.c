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
	unsigned int	nbr_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nbr_of_times_each_philosopher_must_eat;
}	t_options;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					fork_in_hand;
	bool				eating;
	bool				sleeping;
	bool				thinking;
	bool				dead;
	unsigned long long	starving_since;
	int					eat_counter;
}	t_philo;

typedef struct s_fork
{
	int				id;
	bool			used;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_simulation
{
	t_options		options;
	t_philo			**philosophers;
	t_fork			**forks;
	unsigned int	satiated_philo;
	bool			ended;
	pthread_mutex_t	general_mutex;
}	t_simulation;

static t_simulation	g_simulation;

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

t_fork	*create_fork(int id)
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	fork->id = id;
	fork->used = false;
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
	id++;
	if (id == fork_amout)
		id = 0;
	return (forks[id]);
}

t_fork	*get_left_fork(int id, t_fork **forks)
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

void	add_satiated_philo(void)
{
	pthread_mutex_lock(&g_simulation.general_mutex);
	g_simulation.satiated_philo++;
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

void	try_getting_fork(t_philo *me)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(me))
		return ;
	right_fork = get_right_fork(me->id - 1, g_simulation.forks,
			g_simulation.options.nbr_of_philosophers);
	left_fork = get_left_fork(me->id - 1, g_simulation.forks);
	lock_forks(left_fork, right_fork);
	if (!right_fork->used && !left_fork->used)
	{
		print_status(me, "has taken a fork");
		print_status(me, "has taken a fork");
		right_fork->used = true;
		left_fork->used = true;
		me->fork_in_hand = 2;
	}
	unlock_forks(left_fork, right_fork);
}

void	p_eat(t_philo *me)
{
	t_fork	*right_fork;
	t_fork	*left_fork;

	if (end_checker(me))
		return ;
	right_fork = get_right_fork(me->id - 1, g_simulation.forks,
			g_simulation.options.nbr_of_philosophers);
	left_fork = get_left_fork(me->id - 1, g_simulation.forks);
	me->starving_since = get_time_millis();
	print_status(me, "is eating");
	me->eat_counter++;
	if (me->eat_counter
		== g_simulation.options.nbr_of_times_each_philosopher_must_eat)
		add_satiated_philo();
	if (philosopher_sleep(me, g_simulation.options.time_to_eat))
		return ;
	lock_forks(left_fork, right_fork);
	right_fork->used = false;
	left_fork->used = false;
	unlock_forks(left_fork, right_fork);
	me->fork_in_hand = 0;
}

void	p_sleep(t_philo *me)
{
	if (end_checker(me))
		return ;
	print_status(me, "is sleeping");
	philosopher_sleep(me, g_simulation.options.time_to_sleep);
}

void	p_think(t_philo *me)
{
	if (end_checker(me))
		return ;
	print_status(me, "is thinking");
	usleep(1000);
}

void	p_die(t_philo *me)
{
	print_status(me, "died");
}

void	*routine(void *arg)
{
	t_philo	*me;

	me = (t_philo *)arg;
	while (1)
	{
		usleep(100);
		end_checker(me);
		if (me->dead)
		{
			p_die(me);
			break ;
		}
		if (g_simulation.ended)
			break ;
		try_getting_fork(me);
		if (me->fork_in_hand != 2)
			continue ;
		p_eat(me);
		p_sleep(me);
		p_think(me);
	}
	return (NULL);
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

void	create_simulation(t_options options, t_fork **fork_list, t_philo **philo_list)
{
	g_simulation.options = options;
	g_simulation.forks = fork_list;
	g_simulation.philosophers = philo_list;
	g_simulation.satiated_philo = 0;
	pthread_mutex_init(&g_simulation.general_mutex, NULL);
}

void	destroy_simulation(void)
{
	pthread_mutex_destroy(&g_simulation.general_mutex);
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
