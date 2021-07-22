/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 00:57:38 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/07 16:04:50 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		ph_philo_pthread(t_philo *p)
{
	while (1)
	{
		if (p->ctrl->stop_emulation != 0)
			return ;
		ph_print(p, "is thinking");
		pthread_mutex_lock(p->resolve_mt);
		pthread_mutex_lock(p->lf_mt);
		ph_print(p, "has taken a fork");
		pthread_mutex_lock(p->rf_mt);
		ph_print(p, "has taken a fork");
		pthread_mutex_lock(&p->die_time_mt);
		p->die_time = ph_timestamp() + p->ctrl->time_to_die;
		pthread_mutex_unlock(&p->die_time_mt);
		ph_print(p, "is eating");
		ph_sleep(p->ctrl->time_to_eat);
		pthread_mutex_lock(&p->num_eats_mt);
		p->num_eats += 1;
		pthread_mutex_unlock(&p->num_eats_mt);
		pthread_mutex_unlock(p->lf_mt);
		pthread_mutex_unlock(p->rf_mt);
		pthread_mutex_unlock(p->resolve_mt);
		ph_print(p, "is sleeping");
		ph_sleep(p->ctrl->time_to_sleep);
	}
}
