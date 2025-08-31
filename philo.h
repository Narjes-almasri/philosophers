/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:35 by naalmasr          #+#    #+#             */
/*   Updated: 2025/08/31 20:13:36 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING 0
# endif

typedef struct s_philo	t_philo;

/*
1. What is in the data?
data is a pointer to a t_data struct. It holds all the global simulation info, such as:

nb_of_philo: Number of philosophers.
time_to_die, time_to_eat, time_to_sleep: Timing parameters.
must_eat_count: How many times each philosopher must eat (optional).
philos: Array of pointers to all philosopher structs.
fork_locks: Array of mutexes for forks.
sim_stop_lock, write_lock: Mutexes for simulation control and output.
sim_stop: Boolean flag to stop the simulation.*/


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
}	t_data;

typedef struct s_philo
{
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_t			thread;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_data				*data;
}	t_philo;

typedef enum s_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

int		main(int ac, char **av);
int	is_correct_input(int ac, char **av);
int		ft_atoi(char *str);
void	*error_null(char *str, char *details, t_data *data);
int		error_failure(char *str, char *details, t_data *data);
int		msg(char *str, char *detail, int exit_no);
void	destroy_mutexes(t_data *data);
void	*free_table(t_data *data);
t_data	*init_data(int ac, char **av);
void	sim_start_delay(time_t start_time);
void	p_sleep(t_data *data, time_t sleep_time);
time_t	current_time(void);
void	*philosopher(void *data);
void	*monitor(void *src);
void	write_outcome(t_data *data);
void	write_status(t_philo *philo, bool reaper_report, t_status status);
bool	has_simulation_stopped(t_data *data);

#endif