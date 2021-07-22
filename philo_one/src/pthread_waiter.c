/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_waiter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:30:22 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 14:08:47 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

static int		ph_check_died(t_philo *p, int i, t_ctrl *ctrl)
{
	(void)ctrl;
	pthread_mutex_lock(&p[i].die_time_mt);
	if ((ph_timestamp() - p[i].die_time) > 0)
	{
		pthread_mutex_unlock(&p[i].die_time_mt);
		usleep(3000);
		pthread_mutex_lock(&p[i].die_time_mt);
		if ((ph_timestamp() - p[i].die_time) > 0)
		{
			pthread_mutex_unlock(&p[i].die_time_mt);
			ph_print(p, "died");
			return (1);
		}
	}
	pthread_mutex_unlock(&p[i].die_time_mt);
	return (0);
}

void			ph_waiter_loop(t_ctrl *ctrl, t_philo *p)
{
	int				i;
	int				cout_philo_eat_max;

	i = 0;
	cout_philo_eat_max = 0;
	while (1)
	{
		if (i == 0)
		{
			if (cout_philo_eat_max >= ctrl->num_of_philo)
				break ;
			else
				cout_philo_eat_max = 0;
		}
		if (ph_check_died(p, i, ctrl) != 0)
			break ;
		pthread_mutex_lock(&p[i].num_eats_mt);
		if (p[i].num_eats >= ctrl->num_of_meals)
			cout_philo_eat_max++;
		pthread_mutex_unlock(&p[i].num_eats_mt);
		i++;
		i = i % ctrl->num_of_philo;
	}
	ctrl->stop_emulation = 1;
	pthread_mutex_lock(&ctrl->print_mt);
}

int				ph_waiter(t_ctrl *ctrl, t_philo *p, pthread_mutex_t *resolve_mt)
{
	int				exit_delay;
	pthread_t		permissions;
	t_resolve		resolve;

	resolve.ctrl = ctrl;
	resolve.resolve_mt = resolve_mt;
	if (pthread_create(&permissions, NULL,
						(void *(*)(void *))ph_access_control, &resolve) != 0)
		return (ph_error("pthread_create ph_access_control"));
	if (pthread_detach(permissions) != 0)
		return (ph_error("pthread_detach ph_access_control"));
	ph_waiter_loop(ctrl, p);
	exit_delay = ctrl->time_to_sleep + ctrl->time_to_eat;
	ph_sleep(exit_delay);
	return (0);
}
