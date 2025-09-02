/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:35 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/02 21:14:41 by macbook          ###   ########.fr       */
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
data is a pointer to a t_info struct. It holds all the global simulation info, such as:

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
}	t_info ;
/*
 Key Insight: Each philosopher has pointers to fork mutexes, not the mutexes themselves. This allows multiple philosophers to reference the same physical forks!
*/
typedef struct s_philo //Per-philosopher data
{
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_t			thread;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_info				*data;
}	t_philo;
/*struct contains all shared data that multiple threads need to access. Each piece of shared data gets its own mutex for fine-grained locking.*/
typedef enum s_status //shared simulation state cointains actions 
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	TAKING_FORK_1 = 4,
	TAKING_FORK_2 = 5
}	t_status;

int is_simulation_stopped(t_info *info);
int start(int argc, char **argv);
int		is_valid_input(int argc, char **argv);
int		ft_atoi(char *str);
void	*error_null(char *str, char *details, t_info *data);
int		error_failure(char *str, char *details, t_info *data);
int		msg(char *str, char *detail, int exit_no);
void	destroy_mutexes(t_info *data);
void	*free_table(t_info *data);
t_info	*init_data(int ac, char **av);
void	sim_start_delay(time_t start_time);
void	p_sleep(t_info *data, time_t sleep_time);
time_t	current_time(void);
void *philosopher(void *input);
void *monitor(void *src);
void	print_outcome(t_info *data);
void write_status(t_philo *p, int report, t_status st);
// int	has_simulation_stopped(t_info *data);

#endif