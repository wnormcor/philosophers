/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_philo_trf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 00:57:38 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/20 11:19:34 by wnormcor         ###   ########.fr       */
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

void			ph_philo_pthread(t_philo *p)
{
	while (1)
	{
		ph_print(p, "is thinking", 0);
		sem_wait(p->resolve_mt);
		if (p->ctrl->stop_emulation != 0)
			return ;
		sem_wait(p->ctrl->fork_mt);
		ph_print(p, "has taken a fork", 0);
		sem_wait(p->ctrl->fork_mt);
		ph_print(p, "has taken a fork", 0);
		sem_wait(p->die_time_mt);
		p->die_time = ph_timestamp() + p->ctrl->time_to_die;
		sem_post(p->die_time_mt);
		ph_print(p, "is eating", 0);
		ph_sleep(p->ctrl->time_to_eat);
		sem_wait(p->num_eats_mt);
		p->num_eats++;
		sem_post(p->num_eats_mt);
		sem_post(p->ctrl->fork_mt);
		sem_post(p->ctrl->fork_mt);
		sem_post(p->resolve_mt);
		ph_print(p, "is sleeping", 0);
		ph_sleep(p->ctrl->time_to_sleep);
	}
}

static void		give_access(int part, sem_t **resolve_mt, int num, int mode)
{
	int		i;

	i = -1;
	if (part == 0 || part == 1)
	{
		while (++i < ((num % 2 == 0) ? num : num - 1))
			if ((part == 0 && i % 2 == 0) || (part == 1 && i % 2 == 1))
				(mode) ? sem_post(resolve_mt[i]) : sem_wait(resolve_mt[i]);
	}
	else if (part == 2)
		(mode) ? sem_post(resolve_mt[num - 1]) : sem_wait(resolve_mt[num - 1]);
}

void			*ph_trf_pthread(void *arg)
{
	int		part;
	t_ctrl	*p_ctrl;

	p_ctrl = (t_ctrl *)arg;
	part = 0;
	while (1)
	{
		give_access(part, p_ctrl->resolve_mt, p_ctrl->num_of_philo, 1);
		ph_sleep(20);
		if (p_ctrl->stop_emulation != 0)
			break ;
		give_access(part, p_ctrl->resolve_mt, p_ctrl->num_of_philo, 0);
		if (p_ctrl->stop_emulation != 0)
			break ;
		part++;
		part = (p_ctrl->num_of_philo % 2 == 0) ? part % 2 : part % 3;
	}
	part = -1;
	while (++part < p_ctrl->num_of_philo)
		sem_post(p_ctrl->resolve_mt[part]);
	return (NULL);
}
