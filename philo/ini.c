/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:16:01 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:44:19 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

static pthread_mutex_t	*create_fork_mutexes(t_info *info)
{
	pthread_mutex_t	*mutexes;
	unsigned int	idx;

	idx = 0;
	mutexes = malloc(sizeof(pthread_mutex_t) * info->num_philos);
	if (!mutexes)
		return (null_errors("Memory allocation failed for forks.\n", NULL, 0));
	while (idx < info->num_philos)
	{
		if (pthread_mutex_init(&mutexes[idx], 0) != 0)
			return (null_errors("Mutex creation failed for fork.\n", NULL, 0));
		idx++;
	}
	return (mutexes);
}

static void	set_fork_indices(t_philo *ph)
{
	ph->fork[0] = ph->id;
	ph->fork[1] = (ph->id + 1) % ph->data->num_philos;
	if (ph->id % 2)
	{
		ph->fork[0] = (ph->id + 1) % ph->data->num_philos;
		ph->fork[1] = ph->id;
	}
}

static t_philo	**make_philosophers(t_info *info)
{
	t_philo			**philos_arr;
	unsigned int	idx;

	idx = 0;
	philos_arr = malloc(sizeof(t_philo) * info->num_philos);
	if (!philos_arr)
		return (null_errors("Philosopher allocation failed.\n", NULL, 0));
	while (idx < info->num_philos)
	{
		philos_arr[idx] = malloc(sizeof(t_philo));
		if (!philos_arr[idx])
			return (null_errors("Philosopher allocation failed.\n", NULL, 0));
		if (pthread_mutex_init(&philos_arr[idx]->meal_time_lock, 0) != 0)
			return (null_errors("Mutex creation failed for meal time.\n", NULL,
					0));
		philos_arr[idx]->data = info;
		philos_arr[idx]->id = idx;
		philos_arr[idx]->times_ate = 0;
		philos_arr[idx]->last_meal = 0;
		set_fork_indices(philos_arr[idx]);
		idx++;
	}
	return (philos_arr);
}

static int	setup_global_mutexes(t_info *info)
{
	info->fork_locks = create_fork_mutexes(info);
	if (!info->fork_locks)
		return (0);
	if (pthread_mutex_init(&info->sim_stop_lock, 0) != 0)
		return (error_fail("Mutex creation failed.\n", NULL, info));
	if (pthread_mutex_init(&info->write_lock, 0) != 0)
		return (error_fail("Mutex creation failed.\n", NULL, info));
	return (1);
}

t_info	*init_data(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	if (!info)
		return (null_errors("Info allocation failed.\n", NULL, 0));
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->must_eat_count = -1;
	if (argc - 1 == 5)
		info->must_eat_count = ft_atoi(argv[5]);
	info->philos = make_philosophers(info);
	if (!info->philos)
		return (NULL);
	if (!setup_global_mutexes(info))
		return (NULL);
	info->sim_stop = 0;
	return (info);
}
