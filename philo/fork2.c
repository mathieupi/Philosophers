/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:28:01 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:45 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fork.h"

t_fork	*get_right_fork(int id, t_fork **forks, int fork_amout)
{
	return (forks[(id + 1) % fork_amout]);
}

t_fork	*get_left_fork(int id, t_fork **forks)
{
	return (forks[id]);
}
