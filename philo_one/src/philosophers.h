/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 23:20:09 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/07 17:44:22 by wnormcor         ###   ########.fr       */
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
**						pthread_mutex_init
**						pthread_mutex_lock
**						pthread_mutex_unlock
**						pthread_mutex_destroy
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
# include <limits.h>

typedef struct		s_ctrl
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	int				stop_emulation;
	pthread_mutex_t	print_mt;
}					t_ctrl;

typedef struct		s_philo
{
	int				i;
	pthread_t		id;
	pthread_mutex_t	*resolve_mt;
	pthread_mutex_t	*lf_mt;
	pthread_mutex_t	*rf_mt;
	pthread_mutex_t	die_time_mt;
	long long		die_time;
	pthread_mutex_t	num_eats_mt;
	int				num_eats;
	t_ctrl			*ctrl;
}					t_philo;

typedef	struct		s_resolve
{
	t_ctrl			*ctrl;
	pthread_mutex_t	*resolve_mt;
}					t_resolve;

int					ph_error(char *str);
int					ph_atoi(const char *s);
void				*ph_malloc(size_t size);
long long			ph_timestamp();
void				ph_print(t_philo *p_philo, char *msg);
void				ph_sleep(time_t ms);

void				ph_philo_pthread(t_philo *philo);
int					ph_waiter(t_ctrl *ctrl, t_philo *p,
								pthread_mutex_t *resolve_mt);
void				ph_access_control(t_resolve *resolve);

#endif
