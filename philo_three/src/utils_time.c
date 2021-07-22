/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:50:37 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 12:50:47 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long		ph_timestamp(void)
{
	struct timeval			tv;
	static long long int	init_sec = 0;
	static long long int	init_usec = 0;

	gettimeofday(&tv, NULL);
	if (init_sec == 0 && init_usec == 0)
	{
		init_sec = (long long int)tv.tv_sec;
		init_usec = (long long int)tv.tv_usec;
	}
	return ((long long int)(((tv.tv_sec - init_sec) * 1000) \
							+ 0 + ((tv.tv_usec - init_usec) / 1000)));
}

static time_t	ph_take_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((time_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void			ph_sleep(time_t ms)
{
	time_t		t;

	t = ph_take_time();
	usleep(ms * 998);
	while (ph_take_time() < t + ms)
		usleep(ms);
}
