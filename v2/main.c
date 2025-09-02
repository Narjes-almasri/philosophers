/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:53:43 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/02 21:13:37 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Starts the simulation by creating philosopher threads and a monitor thread.
static int	begin_simulation(t_info *info)
{
	unsigned int idx = 0;
	info->start_time = current_time() + (info->num_philos * 2 * 10);
	while (idx < info->num_philos)
	{
		if (pthread_create(&info->philos[idx]->thread, NULL,
				&philosopher, info->philos[idx]) != 0)
			return (error_failure("Thread creation failed.\n", NULL, info));
		idx++;
	}
	if (info->num_philos > 1)
	{
		if (pthread_create(&info->grim_reaper, NULL,
				&monitor, info) != 0)
			return (error_failure("Thread creation failed.\n", NULL, info));
	}
	return (1);
}

static void	end_simulation(t_info *info)
{
	unsigned int idx = 0;
	while (idx < info->num_philos)
	{
		pthread_join(info->philos[idx]->thread, NULL);
		idx++;
	}
	if (info->num_philos > 1)
		pthread_join(info->grim_reaper, NULL);
	if (DEBUG_FORMATTING == 0 && info->must_eat_count != -1)
		print_outcome(info);
	destroy_mutexes(info);
	free_table(info);
}

int start(int argc, char **argv)
{
	t_info *info;
	// int valid = 1;
	if (argc < 5 || argc > 6)
	{
		printf("Wrong argument count!\n");
		return 1;
	}
	if (!is_valid_input(argc, argv))
		return 1;
	info = init_data(argc, argv);
	if (!info)
		return 1;
	if (!begin_simulation(info))
		return 1;
	end_simulation(info);
	return 0;
}

int main(int argc, char **argv)
{
	int result = start(argc, argv);
	return result;
}

