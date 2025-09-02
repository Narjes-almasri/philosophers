/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_simulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:00 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/01 17:03:16 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static void	set_sim_stop_flag(t_data *data, bool state)
{
	pthread_mutex_lock(&data->sim_stop_lock);
	data->sim_stop = state;
	pthread_mutex_unlock(&data->sim_stop_lock);
}

int	has_simulation_stopped(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->sim_stop_lock);
	if (data->sim_stop == 1)
		i = 1;
	pthread_mutex_unlock(&data->sim_stop_lock);
	return (i);
}

static int	kill_philo(t_philo *philo)
{
	time_t	time;

	time = current_time();
	if ((time - philo->last_meal) >= philo->data->time_to_die)
	{
		set_sim_stop_flag(philo->data, 1);
		write_status(philo, 1, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (1);
	}
	return (0);
}

static int	end_condition_reached(t_data *data)
{
	unsigned int	i;
	int			all_ate_enough;

	all_ate_enough = 1;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i]->meal_time_lock);
		if (kill_philo(data->philos[i]))
			return (1);
		if (data->must_eat_count != -1)
			if (data->philos[i]->times_ate
				< (unsigned int)data->must_eat_count)
				all_ate_enough = 0;
		pthread_mutex_unlock(&data->philos[i]->meal_time_lock);
		i++;
	}
	if (data->must_eat_count != -1 && all_ate_enough == 1)
	{
		set_sim_stop_flag(data, 1);
		return (1);
	}
	return (0);
}

void	*monitor(void *src)
{
	t_data			*data;

	data = (t_data *)src;
	if (data->must_eat_count == 0)
		return (NULL);
	set_sim_stop_flag(data, 0);
	sim_start_delay(data->start_time);
	while (1)
	{
		if (end_condition_reached(data) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
