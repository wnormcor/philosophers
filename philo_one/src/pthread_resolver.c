/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_resolver.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 16:12:34 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/07 17:54:30 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	give_access(int mode, pthread_mutex_t *resolve_mt, int num)
{
	int		i;
	int		last_elem;

	last_elem = (num % 2 == 0) ? num : num - 1;
	i = -1;
	if (mode == 0)
	{
		while (++i < last_elem)
			if (i % 2 == 0)
				pthread_mutex_unlock(resolve_mt + i);
	}
	else if (mode == 1)
	{
		while (++i < last_elem)
			if (i % 2 != 0)
				pthread_mutex_unlock(resolve_mt + i);
	}
	else if (mode == 2)
		pthread_mutex_unlock(resolve_mt + num - 1);
}

static void	block_access(int mode, pthread_mutex_t *resolve_mt, int num)
{
	int		i;
	int		last_elem;

	last_elem = (num % 2 == 0) ? num : num - 1;
	i = -1;
	if (mode == 0)
	{
		while (++i < last_elem)
			if (i % 2 == 0)
				pthread_mutex_lock(resolve_mt + i);
	}
	else if (mode == 1)
	{
		while (++i < last_elem)
			if (i % 2 != 0)
				pthread_mutex_lock(resolve_mt + i);
	}
	else if (mode == 2)
		pthread_mutex_lock(resolve_mt + num - 1);
}

static void	free_access(pthread_mutex_t *resolve_mt, int num)
{
	int		i;

	i = -1;
	while (++i < num)
		pthread_mutex_unlock(resolve_mt + i);
}

void		ph_access_control(t_resolve *resolve)
{
	int				mode;

	mode = 0;
	while (1)
	{
		give_access(mode, resolve->resolve_mt, resolve->ctrl->num_of_philo);
		ph_sleep(20);
		if (resolve->ctrl->stop_emulation != 0)
			break ;
		block_access(mode, resolve->resolve_mt, resolve->ctrl->num_of_philo);
		if (resolve->ctrl->stop_emulation != 0)
			break ;
		mode++;
		mode = (resolve->ctrl->num_of_philo % 2 == 0) ? mode % 2 : mode % 3;
	}
	free_access(resolve->resolve_mt, resolve->ctrl->num_of_philo);
}
