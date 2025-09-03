/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 08:59:03 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:10:50 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int					must_eat_count;
	bool				sim_stop;
	unsigned int		num_philos;
	time_t				start_time;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	pthread_t			grim_reaper;
	pthread_mutex_t		sim_stop_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	t_philo				**philos;
}	t_info;

typedef struct s_philo
{
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_t			thread;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_info				*data;
}	t_philo;

typedef enum s_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	TAKING_FORK_1 = 4,
	TAKING_FORK_2 = 5
}	t_status;

void	perform_eat_sleep(t_philo *p);
int		is_simulation_stopped(t_info *info);
int		start(int argc, char **argv);
int		begin_sim(t_info *info);
void	end_sim(t_info *info);
void	do_think(t_philo *p, int quiet);
void	update_sim_stop(t_info *info, int flag);
int		is_valid_input(int argc, char **argv);
int		simulation_should_end(t_info *info);
int		ft_atoi(char *str);
void	*null_errors(char *str, char *details, t_info *data);
int		error_fail(char *str, char *details, t_info *data);
int		message(char *str, char *detail, int exit_no);
void	destroy_mutexes(t_info *data);
void	*free_table(t_info *data);
t_info	*init_data(int ac, char **av);
void	sim_start_delay(time_t start_time);
void	p_sleep(t_info *data, time_t sleep_time);
time_t	current_time(void);
void	*philosopher(void *input);
void	*monitoooor(void *src);
void	print_outcome(t_info *data);
void	write_status(t_philo *p, int report, t_status st);

#endif