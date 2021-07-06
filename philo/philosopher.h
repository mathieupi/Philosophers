#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>

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

unsigned long long	get_time_millis(void);
bool				is_digit(char c);
int					parse_int(const char *str);
t_philo				*create_philosopher(int id);
t_philo				**create_philosophers(int amount);
void				start_philosophers(t_philo **philo_list, int amout);
void				remove_philosophers(t_philo **philo_list, int amout);
#endif
