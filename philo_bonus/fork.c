/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:28:05 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:42 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fork.h"

t_fork	*create_fork(int id)
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	fork->id = id;
	fork->used = false;
	pthread_mutex_init(&fork->mutex, NULL);
	return (fork);
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
