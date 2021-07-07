/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:28:21 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:35 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H
# include "philosopher_types.h"
# include "philosopher.h"
# include "fork.h"

void	start_simulation(t_options options);
void	add_satiated_philo(t_simulation *simulation);

#endif
