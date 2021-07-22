/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 23:19:24 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 14:09:14 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	ph_free(t_ctrl *ctrl, t_philo *p_philo,
					pthread_mutex_t *resolve_mt, pthread_mutex_t *fork)
{
	int		i;

	pthread_mutex_destroy(&ctrl->print_mt);
	i = -1;
	while (++i < ctrl->num_of_philo)
	{
		pthread_mutex_destroy(resolve_mt + i);
		pthread_mutex_destroy(fork + i);
		pthread_mutex_destroy(&p_philo[i].die_time_mt);
		pthread_mutex_destroy(&p_philo[i].num_eats_mt);
	}
	free(fork);
	free(resolve_mt);
	free(p_philo);
}

static int	match_philo_mt(t_ctrl *ctrl, t_philo *p,
					pthread_mutex_t *resolve_mt, pthread_mutex_t *fork_mt)
{
	int	i;

	i = -1;
	while (++i < ctrl->num_of_philo)
	{
		p[i].i = i;
		p[i].resolve_mt = resolve_mt + i;
		p[i].lf_mt = fork_mt + i;
		p[i].rf_mt = fork_mt + ((i + 1) % ctrl->num_of_philo);
		if (pthread_mutex_init(&p[i].die_time_mt, NULL) != 0)
			return (ph_error("mutex_init die_time_mt"));
		p[i].die_time = ctrl->time_to_die;
		if (pthread_mutex_init(&p[i].num_eats_mt, NULL) != 0)
			return (ph_error("mutex_init num_eats_mt"));
		p[i].num_eats = 0;
		p[i].ctrl = ctrl;
	}
	return (0);
}

static int	philo_init(t_ctrl *p_ctrl, t_philo **p,
					pthread_mutex_t **resolve_mt, pthread_mutex_t **fork_mt)
{
	int	i;

	*p = ph_malloc(sizeof(t_philo) * p_ctrl->num_of_philo);
	*resolve_mt = ph_malloc(sizeof(pthread_mutex_t) * p_ctrl->num_of_philo);
	*fork_mt = ph_malloc(sizeof(pthread_mutex_t) * p_ctrl->num_of_philo);
	if (*p == NULL || *resolve_mt == NULL || *fork_mt == NULL)
		return (ph_error("ph_malloc error with t_philo/resolve_mt/fork_mt"));
	i = -1;
	if (match_philo_mt(p_ctrl, *p, *resolve_mt, *fork_mt))
		return (1);
	while (++i < p_ctrl->num_of_philo)
		if (pthread_mutex_init(*resolve_mt + i, NULL) != 0)
			return (ph_error("mutex_init array of resolve_mt"));
	i = -1;
	while (++i < p_ctrl->num_of_philo)
		if (pthread_mutex_init(*fork_mt + i, NULL) != 0)
			return (ph_error("mutex_init array of fork_mt"));
	return (0);
}

static int	ctrl_init(t_ctrl *p_ctrl, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (ph_error("wrong arguments, expected 4 or 5 arguments"));
	memset(p_ctrl, 0, sizeof(t_ctrl));
	p_ctrl->num_of_philo = ph_atoi(argv[1]);
	p_ctrl->time_to_die = ph_atoi(argv[2]);
	p_ctrl->time_to_eat = ph_atoi(argv[3]);
	p_ctrl->time_to_sleep = ph_atoi(argv[4]);
	p_ctrl->num_of_meals = (argc == 6) ? ph_atoi(argv[5]) : INT_MAX;
	if (p_ctrl->num_of_philo <= 1 || p_ctrl->num_of_philo > 200)
		return (ph_error("wrong number of philosophers (need 1-200)"));
	if (p_ctrl->time_to_die < 0 || p_ctrl->time_to_eat < 0 ||
		p_ctrl->time_to_sleep < 0 || p_ctrl->num_of_meals < 0)
		return (ph_error("wrong time or num of meals (need >= 0)"));
	if (pthread_mutex_init(&p_ctrl->print_mt, NULL) != 0)
		return (ph_error("pthread_mutex_init with print_mt"));
	return (0);
}

int			main(int argc, char **argv)
{
	int				i;
	t_ctrl			ctrl;
	t_philo			*philo;
	pthread_mutex_t	*resolve_mt;
	pthread_mutex_t	*fork_mt;

	if (ctrl_init(&ctrl, argc, argv))
		return (1);
	if (philo_init(&ctrl, &philo, &resolve_mt, &fork_mt))
		return (1);
	i = -1;
	while (++i < ctrl.num_of_philo)
		pthread_mutex_lock(resolve_mt + i);
	i = -1;
	while (++i < ctrl.num_of_philo)
	{
		if (pthread_create(&philo[i].id, NULL,
						(void *(*)(void *))ph_philo_pthread, &philo[i]) != 0)
			return (ph_error("pthread_create with ph_philo_pthread"));
		if (pthread_detach(philo[i].id) != 0)
			return (ph_error("pthread_detach with ph_philo_pthread"));
	}
	ph_waiter(&ctrl, philo, resolve_mt);
	ph_free(&ctrl, philo, resolve_mt, fork_mt);
	return (0);
}
