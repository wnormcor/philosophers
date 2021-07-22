/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 23:20:09 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/20 11:33:54 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** External functs from subject:
**
**		<stdlib.h>		malloc
**						free
** 		<unistd.h>		write
**						usleep
** 		<string.h>		memset
** 		<stdio.h>		printf
** 		<sys/time.h>	gettimeofday(struct timeval *tv, struct timezone *tz);
** 		<pthread.h>		pthread_create
**						pthread_detach
**						pthread_join
**
**		struct timeval {
**			long	tv_sec;		(seconds)
**			long    tv_usec;	(microsecond)
**		};
*/

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <limits.h>

typedef struct		s_ctrl
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	int				stop_emulation;
	char			**sem_names;
	sem_t			**resolve_mt;
	sem_t			*print_mt;
	sem_t			*fork_mt;
	pthread_t		trf_pth_id;
}					t_ctrl;

typedef struct		s_philo
{
	int				i;
	pthread_t		id;
	char			*resolve_mt_name;
	sem_t			*resolve_mt;
	char			*die_time_mt_name;
	sem_t			*die_time_mt;
	long long		die_time;
	char			*num_eats_mt_name;
	sem_t			*num_eats_mt;
	int				num_eats;
	t_ctrl			*ctrl;
}					t_philo;

int					ph_error(char *str);
int					ph_atoi(const char *s);
void				*ph_malloc(size_t size);
long long			ph_timestamp();
void				ph_print(t_philo *p_philo, char *msg, int stop);
void				ph_sleep(time_t ms);
int					ph_sem_make(char *name, int size, sem_t **result);

void				ph_philo_pthread(t_philo *philo);
void				*ph_trf_pthread(void *arg);
void				ph_waiter_loop(t_ctrl *ctrl, t_philo *p);

#endif
