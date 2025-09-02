/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:53:52 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/01 17:03:10 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//Allocate and initialize one mutex per fork.
static pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!forks)
		return (error_null("%s error: Could not allocate memory.\n", NULL, 0));
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_null("%s error: Could not create mutex.\n", NULL, 0));
		i++;
	}
	return (forks);
}
//The ft_min/ft_max assignment is the key to preventing deadlock!
static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->data->num_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->data->num_philos;
		philo->fork[1] = philo->id;
	}
}

static t_philo	**initialize_philos(t_data *data)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!philos)
		return (error_null("%sCould not allocate memory.\n", NULL, 0));
	i = 0;
	while (i < data->num_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null("%sCould not allocate memory.\n", NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (error_null("%sCould not create mutex.\n", NULL, 0));
		philos[i]->data = data;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		philos[i]->last_meal = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static int	init_global_mutexes(t_data *data)
{
	data->fork_locks = init_forks(data);
	if (!data->fork_locks)
		return (0);
	if (pthread_mutex_init(&data->sim_stop_lock, 0) != 0)
		return (error_failure("%sCould not create mutex.\n", NULL, data));
	if (pthread_mutex_init(&data->write_lock, 0) != 0)
		return (error_failure("%sCould not create mutex.\n", NULL, data));
	return (1);
}

t_data	*init_data(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (error_null("%s error: Could not allocate memory.\n", NULL, 0));
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->must_eat_count = -1;
	if (ac - 1 == 5)
		data->must_eat_count = ft_atoi(av[5]);
	data->philos = initialize_philos(data);
	if (!data->philos)
		return (NULL);
	if (!init_global_mutexes(data))
		return (NULL);
	data->sim_stop = 0;
	return (data);
}
