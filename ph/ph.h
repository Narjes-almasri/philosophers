#ifndef PHELO_H
#define PHELO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct table
{
	int stop_execution;
	int num_of_meals;
	unsigned int num_of_philos;
	time_t time_to_sleep;
	time_t time_to_die;
	time_t time_to_eat;
	time_t start_time;
	pthread_t			grim_reaper;
	pthread_mutex_t		sim_stop_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	t_philo				**philos;

}t_table;

typedef struct philo
{
	t_table		*table;
	time_t		last_meal;
	pthread_t	thread;
	pthread_mutex_t		meal_lock;
	unsigned int		id;
	unsigned int		eaten_times;
	unsigned int		fork[2];
}t_philo;

#endif