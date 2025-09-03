/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_and_sim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:16:55 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:45:06 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

time_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	p_sleep(t_info *data, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = current_time() + sleep_time;
	while (current_time() < wake_up)
	{
		if (is_simulation_stopped(data))
			break ;
		usleep(100);
	}
}

void	*monitoooor(void *src)
{
	t_info	*info;

	info = (t_info *)src;
	if (info->must_eat_count == 0)
		return (NULL);
	update_sim_stop(info, 0);
	sim_start_delay(info->start_time);
	while (1)
	{
		if (simulation_should_end(info) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	begin_sim(t_info *info)
{
	unsigned int	idx;

	idx = 0;
	info->start_time = current_time() + (info->num_philos * 2 * 10);
	while (idx < info->num_philos)
	{
		if (pthread_create(&info->philos[idx]->thread, NULL, &philosopher,
				info->philos[idx]) != 0)
			return (error_fail("Thread creation failed.\n", NULL, info));
		idx++;
	}
	if (info->num_philos > 1)
	{
		if (pthread_create(&info->grim_reaper, NULL, &monitoooor, info) != 0)
			return (error_fail("Thread creation failed.\n", NULL, info));
	}
	return (1);
}

void	end_sim(t_info *info)
{
	unsigned int	idx;

	idx = 0;
	while (idx < info->num_philos)
	{
		pthread_join(info->philos[idx]->thread, NULL);
		idx++;
	}
	if (info->num_philos > 1)
		pthread_join(info->grim_reaper, NULL);
	if (info->must_eat_count != -1)
		print_outcome(info);
	destroy_mutexes(info);
	free_table(info);
}
