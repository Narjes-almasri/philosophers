/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:23 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/02 21:11:29 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static void perform_eat_sleep(t_philo *p)
{
	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
	write_status(p, 0, TAKING_FORK_1);
	pthread_mutex_lock(&p->data->fork_locks[p->fork[1]]);
	write_status(p, 0, TAKING_FORK_2);
	write_status(p, 0, EATING);
	pthread_mutex_lock(&p->meal_time_lock);
	p->last_meal = current_time();
	pthread_mutex_unlock(&p->meal_time_lock);
	p_sleep(p->data, p->data->time_to_eat);
	if (is_simulation_stopped(p->data) == 0)
	{
		pthread_mutex_lock(&p->meal_time_lock);
		p->times_ate += 1;
		pthread_mutex_unlock(&p->meal_time_lock);
	}
	write_status(p, 0, SLEEPING);
	pthread_mutex_unlock(&p->data->fork_locks[p->fork[1]]);
	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
	p_sleep(p->data, p->data->time_to_sleep);
}

static void do_think(t_philo *p, int quiet)
{
	time_t think_time;
	pthread_mutex_lock(&p->meal_time_lock);
	think_time = (p->data->time_to_die
			- (current_time() - p->last_meal)
			- p->data->time_to_eat) / 2;
	pthread_mutex_unlock(&p->meal_time_lock);
	if (think_time < 0)
		think_time = 0;
	if (think_time == 0 && quiet == 1)
		think_time = 1;
	if (think_time > 600)
		think_time = 200;
	if (quiet == 0)
		write_status(p, 0, THINKING);
	p_sleep(p->data, think_time);
}

static void *single_philo(void *arg)
{
	t_philo *p = (t_philo *)arg;
	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
	write_status(p, 0, TAKING_FORK_1);
	p_sleep(p->data, p->data->time_to_die);
	write_status(p, 0, DIED);
	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
	return NULL;
}

void *philosopher(void *input)
{
	t_philo *p = (t_philo *)input;
	if (p->data->must_eat_count == 0)
		return NULL;
	pthread_mutex_lock(&p->meal_time_lock);
	p->last_meal = p->data->start_time;
	pthread_mutex_unlock(&p->meal_time_lock);
	sim_start_delay(p->data->start_time);
	if (p->data->time_to_die == 0)
		return NULL;
	if (p->data->num_philos == 1)
		return single_philo(p);
	else if (p->id % 2)
		do_think(p, 1);
	while (is_simulation_stopped(p->data) == 0)
	{
		perform_eat_sleep(p);
		do_think(p, 0);
	}
	return NULL;
}
