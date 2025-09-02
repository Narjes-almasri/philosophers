/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:00 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/02 21:11:04 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void debug_status_print(t_philo *p, char *msg, t_status st)
{
	if (st == TAKING_FORK_1)
		printf("[%-10ld] Philosopher %d %s (fork %d)\n",
			current_time() - p->data->start_time,
			p->id + 1, msg, p->fork[0]);
	else if (st == TAKING_FORK_2)
		printf("[%-10ld] Philosopher %d %s (fork %d)\n",
			current_time() - p->data->start_time,
			p->id + 1, msg, p->fork[1]);
	else
		printf("[%-10ld] Philosopher %d %s\n",
			current_time() - p->data->start_time,
			p->id + 1, msg);
}

static void debug_write_status(t_philo *p, t_status st)
{
	if (st == DIED)
		debug_status_print(p, "died", st);
	else if (st == EATING)
		debug_status_print(p, "is eating", st);
	else if (st == SLEEPING)
		debug_status_print(p, "is sleeping", st);
	else if (st == THINKING)
		debug_status_print(p, "is thinking", st);
	else if (st == TAKING_FORK_1)
		debug_status_print(p, "grabbed a fork", st);
	else if (st == TAKING_FORK_2)
		debug_status_print(p, "grabbed a fork", st);
}

static void status_print(t_philo *p, char *msg)
{
	printf("%ld %d %s\n", current_time() - p->data->start_time,
		p->id + 1, msg);
}

void write_status(t_philo *p, int report, t_status st)
{
	pthread_mutex_lock(&p->data->write_lock);
	if (is_simulation_stopped(p->data) == 1 && report == 0)
	{
		pthread_mutex_unlock(&p->data->write_lock);
		return;
	}
	if (DEBUG_FORMATTING == 1)
	{
		debug_write_status(p, st);
		pthread_mutex_unlock(&p->data->write_lock);
		return;
	}
	if (st == DIED)
		status_print(p, "died");
	else if (st == EATING)
		status_print(p, "is eating");
	else if (st == SLEEPING)
		status_print(p, "is sleeping");
	else if (st == THINKING)
		status_print(p, "is thinking");
	else if (st == TAKING_FORK_1 || st == TAKING_FORK_2)
		status_print(p, "grabbed a fork");
	pthread_mutex_unlock(&p->data->write_lock);
}

void print_outcome(t_info *info)
{
	unsigned int idx = 0;
	unsigned int satisfied = 0;
	while (idx < info->num_philos)
	{
		if (info->philos[idx]->times_ate >= (unsigned int)info->must_eat_count)
			satisfied++;
		idx++;
	}
	pthread_mutex_lock(&info->write_lock);
	printf("%d/%d philosophers ate at least %d times.\n",
		satisfied, info->num_philos, info->must_eat_count);
	pthread_mutex_unlock(&info->write_lock);
	return;
}
