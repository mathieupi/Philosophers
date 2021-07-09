/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 00:28:23 by mmehran           #+#    #+#             */
/*   Updated: 2021/07/08 00:28:27 by mmehran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

unsigned long long	get_time_millis(void)
{
	struct timeval	tval;

	gettimeofday(&tval, NULL);
	return (tval.tv_sec * 1000 + tval.tv_usec / 1000);
}

bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	parse_int(const char *str)
{
	int	result;

	if (!str)
		return (0);
	result = 0;
	while (is_digit(*str))
		result = result * 10 + (*(str++) - '0');
	return (result);
}
