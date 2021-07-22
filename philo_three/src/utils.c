/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:48:47 by wnormcor          #+#    #+#             */
/*   Updated: 2021/02/22 12:54:03 by wnormcor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				ph_error(char *str)
{
	printf("An error has occurred");
	if (str)
		printf(": %s", str);
	printf(".\n");
	exit(1);
	return (1);
}

void			*ph_malloc(size_t size)
{
	void	*result;

	result = NULL;
	result = malloc(size);
	if (result == NULL)
		return (NULL);
	memset(result, '\0', size);
	return (result);
}

int				ph_atoi(const char *s)
{
	int		sign;
	long	r;
	int		lend;

	r = 0;
	sign = 1;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -1;
	lend = 0;
	while (*s >= '0' && *s <= '9' && ++lend)
		r = r * 10 + *s++ - '0';
	if (lend > 19 && sign == -1)
		return (0);
	if (lend > 19 && sign == 1)
		return (-1);
	return (sign * (int)r);
}

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

void			ph_print(t_philo *p_philo, char *msg, int stop)
{
	sem_wait(p_philo->ctrl->print_mt);
	printf("%lli %i %s\n", ph_timestamp(), p_philo->i + 1, msg);
	if (stop == 1)
	{
		sem_post(p_philo->ctrl->die_mt);
		return ;
	}
	sem_post(p_philo->ctrl->print_mt);
}
