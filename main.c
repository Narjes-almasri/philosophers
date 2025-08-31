/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:53:43 by naalmasr          #+#    #+#             */
/*   Updated: 2025/08/31 19:37:47 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Starts the simulation by creating philosopher threads and a monitor thread.
static bool	start_sim(t_data *data)
{
	unsigned int	i;

	data->start_time = current_time() + (data->num_philos * 2 * 10);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i]->thread, NULL,
				&philosopher, data->philos[i]) != 0)
			return (error_failure("%sCould not create thread.\n", NULL, data));
		i++;
	}
	if (data->num_philos > 1)
	{
		if (pthread_create(&data->grim_reaper, NULL,
				&monitor, data) != 0)
			return (error_failure("%sCould not create thread.\n", NULL, data));
	}
	return (true);
}

static void	stop_sim(t_data	*data)
{
	unsigned int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i]->thread, NULL);
		i++;
	}
	if (data->num_philos > 1)
		pthread_join(data->grim_reaper, NULL);
	if (DEBUG_FORMATTING == 0 && data->must_eat_count != -1)
		write_outcome(data);
	destroy_mutexes(data);
	free_table(data);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac < 5 || ac > 6)
	{
		printf("invaled number of arguments!!!!!!!!🩴\n");
		return (1);
	}
	if (!is_correct_input(ac, av))
		return (1);
	data = init_data(ac, av);
	if (!data)
		return (1);
	if (!start_sim(data))
		return (1);
	stop_sim(data);
	return (0);
}

