/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kapellmeister.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 11:31:42 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 12:56:24 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		ph_free(t_ctrl *ctrl, t_philo *p_philo)
{
	int		i;

	ph_sleep(ctrl->time_to_sleep + ctrl->time_to_eat);
	sem_close(ctrl->print_mt);
	sem_unlink("/LCK_PRINT");
	sem_close(ctrl->fork_mt);
	sem_unlink("/PHILO_FORKS");
	sem_close(ctrl->die_mt);
	sem_unlink("/SOME_DIED");
	sem_close(ctrl->eat_mt);
	sem_unlink("/MAX_EATS");
	i = -1;
	while (++i < ctrl->num_of_philo * 3)
	{
		sem_unlink(ctrl->sem_names[i]);
		free(ctrl->sem_names[i]);
	}
	free(ctrl->resolve_mt);
	free(ctrl->sem_names);
	free(p_philo);
}

static void	*pth_all_eat(void *args)
{
	int			i;
	t_ctrl		*p_ctrl;

	p_ctrl = (t_ctrl *)args;
	i = -1;
	while (++i < p_ctrl->num_of_philo)
		sem_wait(p_ctrl->eat_mt);
	p_ctrl->stop_emulation = 1;
	sem_post(p_ctrl->die_mt);
	return (NULL);
}

static void	*pth_some_die(void *args)
{
	int			i;
	t_ctrl		*p_ctrl;

	p_ctrl = (t_ctrl *)args;
	sem_wait(p_ctrl->die_mt);
	p_ctrl->stop_emulation = 1;
	i = -1;
	while (++i < p_ctrl->num_of_philo)
		sem_post(p_ctrl->eat_mt);
	return (NULL);
}

int			kapellmeister(t_ctrl *ctrl, t_philo *p)
{
	int i;

	if (pthread_create(&(ctrl->trf_pth_id), NULL, ph_trf_pthread, ctrl) != 0)
		return (ph_error("pthread_create ph_access_control"));
	if (pthread_create(&(ctrl->eat_pth_id), NULL, pth_all_eat, ctrl) != 0)
		return (ph_error("pthread_create monitor_max_eats"));
	if (pthread_create(&(ctrl->die_pth_id), NULL, pth_some_die, ctrl) != 0
		&& pthread_detach(ctrl->die_pth_id) != 0)
		return (ph_error("pthread_create monitor_die"));
	while (1)
	{
		if (ctrl->stop_emulation != 0)
			break ;
	}
	pthread_join(ctrl->die_pth_id, NULL);
	pthread_join(ctrl->eat_pth_id, NULL);
	i = -1;
	while (++i < ctrl->num_of_philo)
		kill(p[i].id, SIGKILL);
	return (0);
}
