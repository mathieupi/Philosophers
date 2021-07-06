#include "philosopher.h"

unsigned long long	get_time_millis(void)
{
	struct timeval	tval;

	gettimeofday(&tval, NULL);
	return (tval.tv_sec * 1000 + tval.tv_usec / 1000);
}

int	parse_int(const char *str)
{
	int	result;

	result = 0;
	while (str && *str >= '0' && *str <= '9')
		result = result * 10 + (*(str++) - '0');
	return (result);
}
