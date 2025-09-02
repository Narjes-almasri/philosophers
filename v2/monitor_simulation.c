/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_simulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:00 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/02 21:00:12 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static void update_sim_stop(t_info *info, bool flag)
{
	pthread_mutex_lock(&info->sim_stop_lock);
	info->sim_stop = flag;
	pthread_mutex_unlock(&info->sim_stop_lock);
}

int is_simulation_stopped(t_info *info)
{
	int stopped = 0;
	pthread_mutex_lock(&info->sim_stop_lock);
	if (info->sim_stop == 1)
		stopped = 1;
	pthread_mutex_unlock(&info->sim_stop_lock);
	return stopped;
}

static int check_philo_death(t_philo *p)
{
	time_t now = current_time();
	if ((now - p->last_meal) >= p->data->time_to_die)
	{
		update_sim_stop(p->data, 1);
		write_status(p, 1, DIED);
		pthread_mutex_unlock(&p->meal_time_lock);
		return 1;
	}
	return 0;
}

static int simulation_should_end(t_info *info)
{
	unsigned int idx = 0;
	int everyone_ate = 1;
	while (idx < info->num_philos)
	{
		pthread_mutex_lock(&info->philos[idx]->meal_time_lock);
		if (check_philo_death(info->philos[idx]))
			return 1;
		if (info->must_eat_count != -1)
			if (info->philos[idx]->times_ate < (unsigned int)info->must_eat_count)
				everyone_ate = 0;
		pthread_mutex_unlock(&info->philos[idx]->meal_time_lock);
		idx++;
	}
	if (info->must_eat_count != -1 && everyone_ate == 1)
	{
		update_sim_stop(info, 1);
		return 1;
	}
	return 0;
}

void *monitor(void *src)
{
	t_info *info = (t_info *)src;
	if (info->must_eat_count == 0)
		return NULL;
	update_sim_stop(info, 0);
	sim_start_delay(info->start_time);
	while (1)
	{
		if (simulation_should_end(info) == 1)
			return NULL;
		usleep(1000);
	}
	return NULL;
}
