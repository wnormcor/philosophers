/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:48:47 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/20 11:34:19 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				ph_error(char *str)
{
	printf("An error has occurred");
	if (str)
		printf(": %s", str);
	printf(".\n");
	return (1);
}

void			*ph_malloc(size_t size)
{
	void	*result;

	result = NULL;
	result = malloc(size);
	if (result == NULL)
		return (NULL);
	memset(result, '\0', size);
	return (result);
}

int				ph_atoi(const char *s)
{
	int		sign;
	long	r;
	int		lend;

	r = 0;
	sign = 1;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -1;
	lend = 0;
	while (*s >= '0' && *s <= '9' && ++lend)
		r = r * 10 + *s++ - '0';
	if (lend > 19 && sign == -1)
		return (0);
	if (lend > 19 && sign == 1)
		return (-1);
	return (sign * (int)r);
}

long long		ph_timestamp(void)
{
	static long long	init_ts = 0;
	long long			cur_ts;
	struct timeval		s_tv;

	gettimeofday(&s_tv, NULL);
	cur_ts = (long long)(s_tv.tv_sec * 1000 + s_tv.tv_usec / 1000);
	if (init_ts == 0)
		init_ts = cur_ts;
	return (cur_ts - init_ts);
}

void			ph_print(t_philo *p_philo, char *msg, int stop)
{
	sem_wait(p_philo->ctrl->print_mt);
	if (p_philo->ctrl->stop_emulation == 0)
	{
		if (stop == 1)
			p_philo->ctrl->stop_emulation = 1;
		printf("%lli %i %s\n", ph_timestamp(), p_philo->i + 1, msg);
	}
	sem_post(p_philo->ctrl->print_mt);
}
