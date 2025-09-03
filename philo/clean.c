/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:00:24 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:13:41 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*free_table(t_info *data)
{
	unsigned int	i;

	if (!data)
		return (NULL);
	if (data->fork_locks)
		free(data->fork_locks);
	if (data->philos)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (data->philos[i])
				free(data->philos[i]);
			i++;
		}
		free(data->philos);
	}
	free(data);
	return (NULL);
}

void	destroy_mutexes(t_info *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->fork_locks[i]);
		pthread_mutex_destroy(&data->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->sim_stop_lock);
}

int	message(char *str, char *detail, int exit_no)
{
	if (detail)
		printf(str, "philo :", detail);
	else
		printf(str, "philo :");
	return (exit_no);
}

int	error_fail(char *str, char *details, t_info *data)
{
	if (data)
		free_table(data);
	return (message(str, details, 0));
}

void	*null_errors(char *str, char *details, t_info *data)
{
	if (data)
		free_table(data);
	message(str, details, 1);
	return (NULL);
}
