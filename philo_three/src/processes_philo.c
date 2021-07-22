/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_philo_trf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 00:57:38 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 12:29:55 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*checker_pthread(void *arg)
{
	t_philo			*p;

	p = (t_philo *)arg;
	sem_wait(p->startchk_mt);
	while (1)
	{
		if ((ph_timestamp() - p->die_time) > 0)
		{
			usleep(200);
			if ((ph_timestamp() - p->die_time) > 0)
			{
				ph_print(p, "died", 1);
				break ;
			}
		}
	}
	return (NULL);
}

static int	ph_philo_loop(t_philo *p)
{
	while (1)
	{
		ph_print(p, "is thinking", 0);
		sem_wait(p->resolve_mt);
		sem_wait(p->ctrl->fork_mt);
		sem_wait(p->ctrl->fork_mt);
		ph_print(p, "has taken a fork", 0);
		ph_print(p, "has taken a fork", 0);
		p->die_time = ph_timestamp() + p->ctrl->time_to_die;
		ph_print(p, "is eating", 0);
		ph_sleep(p->ctrl->time_to_eat);
		p->num_eats++;
		sem_post(p->ctrl->fork_mt);
		sem_post(p->ctrl->fork_mt);
		sem_post(p->resolve_mt);
		if (p->num_eats == p->ctrl->num_of_meals)
		{
			sem_post(p->ctrl->eat_mt);
			return (0);
		}
		ph_print(p, "is sleeping", 0);
		ph_sleep(p->ctrl->time_to_sleep);
	}
	return (0);
}

int			ph_philo_proc(t_philo *p)
{
	if (pthread_create(&p->check_pth_id, NULL, checker_pthread, p) != 0 &&
		pthread_detach(p->check_pth_id) != 0)
		return (ph_error("create checker pthread for philo"));
	sem_wait(p->startphl_mt);
	ph_philo_loop(p);
	return (0);
}

static void	give_access(int part, sem_t **resolve_mt, int num, int mode)
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

void		*ph_trf_pthread(void *arg)
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
		part++;
		part = (p_ctrl->num_of_philo % 2 == 0) ? part % 2 : part % 3;
	}
	return (NULL);
}
