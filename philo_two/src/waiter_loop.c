/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 11:31:42 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/20 11:36:04 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				ph_sem_make(char *name, int size, sem_t **result)
{
	sem_t	*tmp;

	if ((tmp = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, size)) \
																== SEM_FAILED)
	{
		sem_unlink(name);
		if ((tmp = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, size)) \
																== SEM_FAILED)
			return (1);
	}
	*result = tmp;
	return (0);
}

static int		ph_check_died(t_philo *p, int i, t_ctrl *ctrl)
{
	(void)ctrl;
	sem_wait(p[i].die_time_mt);
	if ((ph_timestamp() - p[i].die_time) > 0)
	{
		sem_post(p[i].die_time_mt);
		usleep(3000);
		sem_wait(p[i].die_time_mt);
		if ((ph_timestamp() - p[i].die_time) > 0)
		{
			sem_post(p[i].die_time_mt);
			ph_print(p, "died", 1);
			return (1);
		}
	}
	sem_post(p[i].die_time_mt);
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
		sem_wait(p[i].num_eats_mt);
		if (p[i].num_eats >= ctrl->num_of_meals)
			cout_philo_eat_max++;
		sem_post(p[i].num_eats_mt);
		i++;
		i = i % ctrl->num_of_philo;
	}
	ctrl->stop_emulation = 1;
}
