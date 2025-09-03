/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:08:38 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:42:15 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*single_philo(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
	write_status(p, 0, TAKING_FORK_1);
	p_sleep(p->data, p->data->time_to_die);
	write_status(p, 0, DIED);
	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
	return (NULL);
}

void	*philosopher(void *input)
{
	t_philo	*p;

	p = (t_philo *)input;
	if (p->data->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&p->meal_time_lock);
	p->last_meal = p->data->start_time;
	pthread_mutex_unlock(&p->meal_time_lock);
	sim_start_delay(p->data->start_time);
	if (p->data->time_to_die == 0)
		return (NULL);
	if (p->data->num_philos == 1)
		return (single_philo(p));
	else if (p->id % 2)
		do_think(p, 1);
	while (is_simulation_stopped(p->data) == 0)
	{
		perform_eat_sleep(p);
		do_think(p, 0);
	}
	return (NULL);
}
