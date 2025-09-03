/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:07:55 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 11:06:43 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	status_print(t_philo *p, char *message)
{
	printf("%ld %d %s\n", current_time()
		- p->data->start_time, p->id + 1, message);
}

void	write_status(t_philo *p, int report, t_status st)
{
	pthread_mutex_lock(&p->data->write_lock);
	if (is_simulation_stopped(p->data) == 1 && report == 0)
	{
		pthread_mutex_unlock(&p->data->write_lock);
		return ;
	}
	if (st == DIED)
		status_print(p, "died 🪦");
	else if (st == EATING)
		status_print(p, "is eating 🍝");
	else if (st == SLEEPING)
		status_print(p, "is sleeping 💤");
	else if (st == THINKING)
		status_print(p, "is thinking 🤔");
	else if (st == TAKING_FORK_1 || st == TAKING_FORK_2)
		status_print(p, "grabbed a fork 𐂐");
	pthread_mutex_unlock(&p->data->write_lock);
}

void	print_outcome(t_info *info)
{
	unsigned int	idx;
	unsigned int	satisfied;

	idx = 0;
	satisfied = 0;
	while (idx < info->num_philos)
	{
		if (info->philos[idx]->times_ate >= (unsigned int)info->must_eat_count)
			satisfied++;
		idx++;
	}
	pthread_mutex_lock(&info->write_lock);
	printf("%d/%d philos ate at least %d times.\n",
		satisfied, info->num_philos, info->must_eat_count);
	pthread_mutex_unlock(&info->write_lock);
}
