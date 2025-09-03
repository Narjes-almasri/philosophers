/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:09:48 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:43:36 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	sim_start_delay(time_t start_time)
{
	while (current_time() < start_time)
		continue ;
}

void	update_sim_stop(t_info *info, int flag)
{
	pthread_mutex_lock(&info->sim_stop_lock);
	info->sim_stop = flag;
	pthread_mutex_unlock(&info->sim_stop_lock);
}

int	is_simulation_stopped(t_info *info)
{
	int	stopped;

	stopped = 0;
	pthread_mutex_lock(&info->sim_stop_lock);
	if (info->sim_stop == 1)
		stopped = 1;
	pthread_mutex_unlock(&info->sim_stop_lock);
	return (stopped);
}

int	check_philo_death(t_philo *p)
{
	time_t	now;

	now = current_time();
	if ((now - p->last_meal) >= p->data->time_to_die)
	{
		update_sim_stop(p->data, 1);
		write_status(p, 1, DIED);
		pthread_mutex_unlock(&p->meal_time_lock);
		return (1);
	}
	return (0);
}

int	simulation_should_end(t_info *info)
{
	unsigned int	idx;
	int				everyone_ate;

	idx = 0;
	everyone_ate = 1;
	while (idx < info->num_philos)
	{
		pthread_mutex_lock(&info->philos[idx]->meal_time_lock);
		if (check_philo_death(info->philos[idx]))
			return (1);
		if (info->must_eat_count != -1)
			if (info->philos[idx]->times_ate
				< (unsigned int)info->must_eat_count)
				everyone_ate = 0;
		pthread_mutex_unlock(&info->philos[idx]->meal_time_lock);
		idx++;
	}
	if (info->must_eat_count != -1 && everyone_ate == 1)
	{
		update_sim_stop(info, 1);
		return (1);
	}
	return (0);
}
