/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 23:19:24 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 12:57:20 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				ph_make_arr_named_semaphore(t_ctrl *ctrl)
{
	int		i;
	int		j;
	char	*sig_pref;
	int		nbr;

	ctrl->resolve_mt = ph_malloc(sizeof(sem_t *) * 3 * ctrl->num_of_philo);
	if (ctrl->resolve_mt == NULL)
		return (ph_error("ph_malloc error for pointers to semaphores"));
	sig_pref = "/PH_SEMAPHORE_";
	i = -1;
	while (++i < 3 * ctrl->num_of_philo)
	{
		if ((ctrl->sem_names[i] = ph_malloc(sizeof(char) * 18)) == NULL)
			return (ph_error("ph_malloc error with arr_named_semaphores"));
		nbr = i;
		j = 17;
		while (--j >= 0)
		{
			ctrl->sem_names[i][j] = (j > 13) ? '0' + (nbr % 10) : sig_pref[j];
			nbr /= 10;
		}
		if (ph_sem_make(ctrl->sem_names[i], 1, &ctrl->resolve_mt[i]) != 0)
			return (ph_error("create sem for philo"));
	}
	return (0);
}

static int		philo_init(t_ctrl *p_ctrl, t_philo **p)
{
	int		i;

	if (ph_make_arr_named_semaphore(p_ctrl))
		return (1);
	if ((*p = ph_malloc(sizeof(t_philo) * p_ctrl->num_of_philo)) == NULL)
		return (ph_error("ph_malloc error for t_philo"));
	i = -1;
	while (++i < p_ctrl->num_of_philo)
	{
		(*p)[i].i = i;
		(*p)[i].ctrl = p_ctrl;
		(*p)[i].die_time = p_ctrl->time_to_die;
		(*p)[i].resolve_mt_name = p_ctrl->sem_names[i];
		(*p)[i].resolve_mt = p_ctrl->resolve_mt[i];
		(*p)[i].startchk_mt_name = p_ctrl->sem_names[i + p_ctrl->num_of_philo];
		(*p)[i].startchk_mt = p_ctrl->resolve_mt[i + p_ctrl->num_of_philo];
		(*p)[i].startphl_mt_name = \
							p_ctrl->sem_names[i + p_ctrl->num_of_philo * 2];
		(*p)[i].startphl_mt = p_ctrl->resolve_mt[i + p_ctrl->num_of_philo * 2];
	}
	ph_timestamp();
	return (0);
}

static int		ctrl_init(t_ctrl *p_ctrl, int argc, char **argv)
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
	p_ctrl->sem_names = ph_malloc(sizeof(char *) * 3 * p_ctrl->num_of_philo);
	if (p_ctrl->sem_names == NULL)
		return (ph_error("ph_malloc error for names"));
	if (ph_sem_make("/LCK_PRINT", 1, &p_ctrl->print_mt) ||
		ph_sem_make("/PHILO_FORKS", p_ctrl->num_of_philo, &p_ctrl->fork_mt) ||
		ph_sem_make("/SOME_DIED", 1, &p_ctrl->die_mt) ||
		ph_sem_make("/MAX_EATS", p_ctrl->num_of_philo, &p_ctrl->eat_mt))
		return (ph_error("create sem for print and forks"));
	return (0);
}

int				bl_mt(t_ctrl *p_ctrl)
{
	int				i;

	i = -1;
	while (++i < p_ctrl->num_of_philo)
	{
		sem_wait(p_ctrl->resolve_mt[i]);
		sem_wait(p_ctrl->resolve_mt[i + p_ctrl->num_of_philo]);
		sem_wait(p_ctrl->resolve_mt[i + 2 * p_ctrl->num_of_philo]);
		sem_wait(p_ctrl->eat_mt);
	}
	sem_wait(p_ctrl->die_mt);
	return (0);
}

int				main(int ac, char **av)
{
	int				i;
	t_ctrl			ctrl;
	t_philo			*philo;

	if (ctrl_init(&ctrl, ac, av) || philo_init(&ctrl, &philo) || bl_mt(&ctrl))
		return (1);
	;
	i = -1;
	while (++i < ctrl.num_of_philo)
	{
		if ((philo[i].id = fork()) == -1)
			return (ph_error("fork for philo"));
		else if (philo[i].id == 0)
			exit(ph_philo_proc(&philo[i]));
	}
	i = -1;
	while (++i < ctrl.num_of_philo)
	{
		sem_post(ctrl.resolve_mt[i + ctrl.num_of_philo]);
		sem_post(ctrl.resolve_mt[i + 2 * ctrl.num_of_philo]);
	}
	if (kapellmeister(&ctrl, philo))
		return (1);
	ph_free(&ctrl, philo);
	return (0);
}
