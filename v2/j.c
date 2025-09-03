


// #ifndef PHILO_H
// # define PHILO_H

// # include <pthread.h>
// # include <limits.h>
// # include <unistd.h>
// # include <stdio.h>
// # include <stdlib.h>
// # include <sys/time.h>
// # include <stdbool.h>

// # ifndef DEBUG_FORMATTING
// #  define DEBUG_FORMATTING 0
// # endif

// typedef struct s_philo	t_philo;

// /*
// 1. What is in the data?
// data is a pointer to a t_info struct. It holds all the global simulation info, such as:

// nb_of_philo: Number of philosophers.
// time_to_die, time_to_eat, time_to_sleep: Timing parameters.
// must_eat_count: How many times each philosopher must eat (optional).
// philos: Array of pointers to all philosopher structs.
// fork_locks: Array of mutexes for forks.
// sim_stop_lock, write_lock: Mutexes for simulation control and output.
// sim_stop: Boolean flag to stop the simulation.*/


// typedef struct s_data
// {
// 	int					must_eat_count;
// 	bool				sim_stop;
// 	unsigned int		num_philos;
// 	time_t				start_time;
// 	time_t				time_to_die;
// 	time_t				time_to_eat;
// 	time_t				time_to_sleep;
// 	pthread_t			grim_reaper;
// 	pthread_mutex_t		sim_stop_lock;
// 	pthread_mutex_t		write_lock;
// 	pthread_mutex_t		*fork_locks;
// 	t_philo				**philos;
// }	t_info ;
// /*
//  Key Insight: Each philosopher has pointers to fork mutexes, not the mutexes themselves. This allows multiple philosophers to reference the same physical forks!
// */
// typedef struct s_philo //Per-philosopher data
// {
// 	unsigned int		id;
// 	unsigned int		times_ate;
// 	unsigned int		fork[2];
// 	pthread_t			thread;
// 	pthread_mutex_t		meal_time_lock;
// 	time_t				last_meal;
// 	t_info				*data;
// }	t_philo;
// /*struct contains all shared data that multiple threads need to access. Each piece of shared data gets its own mutex for fine-grained locking.*/
// typedef enum s_status //shared simulation state cointains actions 
// {
// 	DIED = 0,
// 	EATING = 1,
// 	SLEEPING = 2,
// 	THINKING = 3,
// 	TAKING_FORK_1 = 4,
// 	TAKING_FORK_2 = 5
// }	t_status;

// int is_simulation_stopped(t_info *info);
// int start(int argc, char **argv);
// int		is_valid_input(int argc, char **argv);
// int		ft_atoi(char *str);
// void	*error_null(char *str, char *details, t_info *data);
// int		error_failure(char *str, char *details, t_info *data);
// int		msg(char *str, char *detail, int exit_no);
// void	destroy_mutexes(t_info *data);
// void	*free_table(t_info *data);
// t_info	*init_data(int ac, char **av);
// void	sim_start_delay(time_t start_time);
// void	p_sleep(t_info *data, time_t sleep_time);
// time_t	current_time(void);
// void *philosopher(void *input);
// void *monitor(void *src);
// void	print_outcome(t_info *data);
// void write_status(t_philo *p, int report, t_status st);
// // int	has_simulation_stopped(t_info *data);

// #endif

// void	*free_table(t_info *data)
// {
// 	unsigned int	i;

// 	if (!data)
// 		return (NULL);
// 	if (data->fork_locks != NULL)
// 		free(data->fork_locks);
// 	if (data->philos != NULL)
// 	{
// 		i = 0;
// 		while (i < data->num_philos)
// 		{
// 			if (data->philos[i] != NULL)
// 				free(data->philos[i]);
// 			i++;
// 		}
// 		free(data->philos);
// 	}
// 	free(data);
// 	return (NULL);
// }
// }
// void	destroy_mutexes(t_info *data)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (i < data->num_philos)
// 	{
// 		pthread_mutex_destroy(&data->fork_locks[i]);
// 		pthread_mutex_destroy(&data->philos[i]->meal_time_lock);
// 		i++;
// 	}
// 	pthread_mutex_destroy(&data->write_lock);
// 	pthread_mutex_destroy(&data->sim_stop_lock);
// }

// int	msg(char *str, char *detail, int exit_no)
// {
// 	if (!detail)
// 		printf(str, "philo :");
// 	else
// 		printf(str, "philo :", detail);
// 	return (exit_no);
// }

// int	error_failure(char *str, char *details, t_info *data)
// {
// 	if (data != NULL)
// 		free_table(data);
// 	return (msg(str, details, 0));
// }

// void	*error_null(char *str, char *details, t_info *data)
// {
// 	if (data != NULL)
// 		free_table(data);
// 	msg(str, details, 1);
// 	return (NULL);
// }

// static int	check_digits(char *str)
// {
// 	int	i;

// 	i = 0;
// 	// printf("str is %s\n", str);
// 	while (str[i])
// 	{
// 		if (str[i] < '0' || str[i] > '9')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int	ft_atoi(char *str)
// {
// 	unsigned long long int	nbr;
// 	int						i;

// 	i = 0;
// 	nbr = 0;
// 	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
// 	{
// 		nbr = nbr * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	if (nbr > INT_MAX)
// 		return (-1);
// 	return ((int)nbr);
// }

// int	is_valid_input(int ac, char **av)
// {
// 	int	i;
// 	int	num;

// 	i = 1;
// 	while (i < ac)
// 	{
// 		num = ft_atoi(av[i]);
// 		// printf("num is %d\n", num);
// 		// printf("check_digits(av[i] is %d\n", check_digits(av[i]));
// 		if ((!check_digits(av[i])) || (i != 1 && num == -1))//why i != 1 because number of philosophers should not be -1 also the 1 (num of philo ) is checked below in the next if
// 		{
// 			printf("🔢 Error : the input must be digits only 🔢\n");
// 			return (0);
// 		}
// 		if (i == 1 && (num <= 0 || num > 200))//to check number of philosophers
// 		{
// 			printf("❌ you are not allowed to create this number of philosophers ❌\n");
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (1);
// }


// //get current time in milliseconds
// time_t	current_time(void)
// {
// 	struct timeval		tv;

// 	gettimeofday(&tv, NULL);
// 	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
// }

// void	p_sleep(t_info *data, time_t sleep_time)
// {
// 	time_t	wake_up;

// 	wake_up = current_time() + sleep_time;
// 	while (current_time() < wake_up)
// 	{
// 		if (is_simulation_stopped(data))
// 			break ;
// 		usleep(100);
// 	}
// }

// void	sim_start_delay(time_t start_time)
// {
// 	while (current_time() < start_time)
// 		continue ;
// }

// static void perform_eat_sleep(t_philo *p)
// {
// 	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
// 	write_status(p, 0, TAKING_FORK_1);
// 	pthread_mutex_lock(&p->data->fork_locks[p->fork[1]]);
// 	write_status(p, 0, TAKING_FORK_2);
// 	write_status(p, 0, EATING);
// 	pthread_mutex_lock(&p->meal_time_lock);
// 	p->last_meal = current_time();
// 	pthread_mutex_unlock(&p->meal_time_lock);
// 	p_sleep(p->data, p->data->time_to_eat);
// 	if (is_simulation_stopped(p->data) == 0)
// 	{
// 		pthread_mutex_lock(&p->meal_time_lock);
// 		p->times_ate += 1;
// 		pthread_mutex_unlock(&p->meal_time_lock);
// 	}
// 	write_status(p, 0, SLEEPING);
// 	pthread_mutex_unlock(&p->data->fork_locks[p->fork[1]]);
// 	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
// 	p_sleep(p->data, p->data->time_to_sleep);
// }

// static void do_think(t_philo *p, int quiet)
// {
// 	time_t think_time;
// 	pthread_mutex_lock(&p->meal_time_lock);
// 	think_time = (p->data->time_to_die
// 			- (current_time() - p->last_meal)
// 			- p->data->time_to_eat) / 2;
// 	pthread_mutex_unlock(&p->meal_time_lock);
// 	if (think_time < 0)
// 		think_time = 0;
// 	if (think_time == 0 && quiet == 1)
// 		think_time = 1;
// 	if (think_time > 600)
// 		think_time = 200;
// 	if (quiet == 0)
// 		write_status(p, 0, THINKING);
// 	p_sleep(p->data, think_time);
// }

// static void *single_philo(void *arg)
// {
// 	t_philo *p = (t_philo *)arg;
// 	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
// 	write_status(p, 0, TAKING_FORK_1);
// 	p_sleep(p->data, p->data->time_to_die);
// 	write_status(p, 0, DIED);
// 	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
// 	return NULL;
// }

// void *philosopher(void *input)
// {
// 	t_philo *p = (t_philo *)input;
// 	if (p->data->must_eat_count == 0)
// 		return NULL;
// 	pthread_mutex_lock(&p->meal_time_lock);
// 	p->last_meal = p->data->start_time;
// 	pthread_mutex_unlock(&p->meal_time_lock);
// 	sim_start_delay(p->data->start_time);
// 	if (p->data->time_to_die == 0)
// 		return NULL;
// 	if (p->data->num_philos == 1)
// 		return single_philo(p);
// 	else if (p->id % 2)
// 		do_think(p, 1);
// 	while (is_simulation_stopped(p->data) == 0)
// 	{
// 		perform_eat_sleep(p);
// 		do_think(p, 0);
// 	}
// 	return NULL;
// }

// static void debug_status_print(t_philo *p, char *msg, t_status st)
// {
// 	if (st == TAKING_FORK_1)
// 		printf("[%-10ld] Philosopher %d %s (fork %d)\n",
// 			current_time() - p->data->start_time,
// 			p->id + 1, msg, p->fork[0]);
// 	else if (st == TAKING_FORK_2)
// 		printf("[%-10ld] Philosopher %d %s (fork %d)\n",
// 			current_time() - p->data->start_time,
// 			p->id + 1, msg, p->fork[1]);
// 	else
// 		printf("[%-10ld] Philosopher %d %s\n",
// 			current_time() - p->data->start_time,
// 			p->id + 1, msg);
// }

// static void debug_write_status(t_philo *p, t_status st)
// {
// 	if (st == DIED)
// 		debug_status_print(p, "died", st);
// 	else if (st == EATING)
// 		debug_status_print(p, "is eating", st);
// 	else if (st == SLEEPING)
// 		debug_status_print(p, "is sleeping", st);
// 	else if (st == THINKING)
// 		debug_status_print(p, "is thinking", st);
// 	else if (st == TAKING_FORK_1)
// 		debug_status_print(p, "grabbed a fork", st);
// 	else if (st == TAKING_FORK_2)
// 		debug_status_print(p, "grabbed a fork", st);
// }

// static void status_print(t_philo *p, char *msg)
// {
// 	printf("%ld %d %s\n", current_time() - p->data->start_time,
// 		p->id + 1, msg);
// }

// void write_status(t_philo *p, int report, t_status st)
// {
// 	pthread_mutex_lock(&p->data->write_lock);
// 	if (is_simulation_stopped(p->data) == 1 && report == 0)
// 	{
// 		pthread_mutex_unlock(&p->data->write_lock);
// 		return;
// 	}
// 	if (DEBUG_FORMATTING == 1)
// 	{
// 		debug_write_status(p, st);
// 		pthread_mutex_unlock(&p->data->write_lock);
// 		return;
// 	}
// 	if (st == DIED)
// 		status_print(p, "died");
// 	else if (st == EATING)
// 		status_print(p, "is eating");
// 	else if (st == SLEEPING)
// 		status_print(p, "is sleeping");
// 	else if (st == THINKING)
// 		status_print(p, "is thinking");
// 	else if (st == TAKING_FORK_1 || st == TAKING_FORK_2)
// 		status_print(p, "grabbed a fork");
// 	pthread_mutex_unlock(&p->data->write_lock);
// }

// void print_outcome(t_info *info)
// {
// 	unsigned int idx = 0;
// 	unsigned int satisfied = 0;
// 	while (idx < info->num_philos)
// 	{
// 		if (info->philos[idx]->times_ate >= (unsigned int)info->must_eat_count)
// 			satisfied++;
// 		idx++;
// 	}
// 	pthread_mutex_lock(&info->write_lock);
// 	printf("%d/%d philosophers ate at least %d times.\n",
// 		satisfied, info->num_philos, info->must_eat_count);
// 	pthread_mutex_unlock(&info->write_lock);
// 	return;
// }


// static void update_sim_stop(t_info *info, bool flag)
// {
// 	pthread_mutex_lock(&info->sim_stop_lock);
// 	info->sim_stop = flag;
// 	pthread_mutex_unlock(&info->sim_stop_lock);
// }

// int is_simulation_stopped(t_info *info)
// {
// 	int stopped = 0;
// 	pthread_mutex_lock(&info->sim_stop_lock);
// 	if (info->sim_stop == 1)
// 		stopped = 1;
// 	pthread_mutex_unlock(&info->sim_stop_lock);
// 	return stopped;
// }

// static int check_philo_death(t_philo *p)
// {
// 	time_t now = current_time();
// 	if ((now - p->last_meal) >= p->data->time_to_die)
// 	{
// 		update_sim_stop(p->data, 1);
// 		write_status(p, 1, DIED);
// 		pthread_mutex_unlock(&p->meal_time_lock);
// 		return 1;
// 	}
// 	return 0;
// }

// static int simulation_should_end(t_info *info)
// {
// 	unsigned int idx = 0;
// 	int everyone_ate = 1;
// 	while (idx < info->num_philos)
// 	{
// 		pthread_mutex_lock(&info->philos[idx]->meal_time_lock);
// 		if (check_philo_death(info->philos[idx]))
// 			return 1;
// 		if (info->must_eat_count != -1)
// 			if (info->philos[idx]->times_ate < (unsigned int)info->must_eat_count)
// 				everyone_ate = 0;
// 		pthread_mutex_unlock(&info->philos[idx]->meal_time_lock);
// 		idx++;
// 	}
// 	if (info->must_eat_count != -1 && everyone_ate == 1)
// 	{
// 		update_sim_stop(info, 1);
// 		return 1;
// 	}
// 	return 0;
// }

// void *monitor(void *src)
// {
// 	t_info *info = (t_info *)src;
// 	if (info->must_eat_count == 0)
// 		return NULL;
// 	update_sim_stop(info, 0);
// 	sim_start_delay(info->start_time);
// 	while (1)
// 	{
// 		if (simulation_should_end(info) == 1)
// 			return NULL;
// 		usleep(1000);
// 	}
// 	return NULL;
// }


// // Starts the simulation by creating philosopher threads and a monitor thread.
// static int	begin_simulation(t_info *info)
// {
// 	unsigned int idx = 0;
// 	info->start_time = current_time() + (info->num_philos * 2 * 10);
// 	while (idx < info->num_philos)
// 	{
// 		if (pthread_create(&info->philos[idx]->thread, NULL,
// 				&philosopher, info->philos[idx]) != 0)
// 			return (error_failure("Thread creation failed.\n", NULL, info));
// 		idx++;
// 	}
// 	if (info->num_philos > 1)
// 	{
// 		if (pthread_create(&info->grim_reaper, NULL,
// 				&monitor, info) != 0)
// 			return (error_failure("Thread creation failed.\n", NULL, info));
// 	}
// 	return (1);
// }

// static void	end_simulation(t_info *info)
// {
// 	unsigned int idx = 0;
// 	while (idx < info->num_philos)
// 	{
// 		pthread_join(info->philos[idx]->thread, NULL);
// 		idx++;
// 	}
// 	if (info->num_philos > 1)
// 		pthread_join(info->grim_reaper, NULL);
// 	if (DEBUG_FORMATTING == 0 && info->must_eat_count != -1)
// 		print_outcome(info);
// 	destroy_mutexes(info);
// 	free_table(info);
// }
// static void	set_fork_indices(t_philo *ph)
// {
// 	ph->fork[0] = ph->id;
// 	ph->fork[1] = (ph->id + 1) % ph->data->num_philos;
// 	if (ph->id % 2)
// 	{
// 		ph->fork[0] = (ph->id + 1) % ph->data->num_philos;
// 		ph->fork[1] = ph->id;
// 	}
// }

// static t_philo	**make_philosophers(t_info *info)
// {
// 	t_philo **philos_arr;
// 	unsigned int idx = 0;
// 	philos_arr = malloc(sizeof(t_philo) * info->num_philos);
// 	if (!philos_arr)
// 		return (error_null("Philosopher allocation failed.\n", NULL, 0));
// 	while (idx < info->num_philos)
// 	{
// 		philos_arr[idx] = malloc(sizeof(t_philo));
// 		if (!philos_arr[idx])
// 			return (error_null("Philosopher allocation failed.\n", NULL, 0));
// 		if (pthread_mutex_init(&philos_arr[idx]->meal_time_lock, 0) != 0)
// 			return (error_null("Mutex creation failed for meal time.\n", NULL, 0));
// 		philos_arr[idx]->data = info;
// 		philos_arr[idx]->id = idx;
// 		philos_arr[idx]->times_ate = 0;
// 		philos_arr[idx]->last_meal = 0;
// 		set_fork_indices(philos_arr[idx]);
// 		idx++;
// 	}
// 	return (philos_arr);
// }

// t_info	*init_data(int argc, char **argv)
// {
// 	t_info *info;
// 	info = malloc(sizeof(t_info));
// 	if (!info)
// 		return (error_null("Info allocation failed.\n", NULL, 0));
// 	info->num_philos = ft_atoi(argv[1]);
// 	info->time_to_die = ft_atoi(argv[2]);
// 	info->time_to_eat = ft_atoi(argv[3]);
// 	info->time_to_sleep = ft_atoi(argv[4]);
// 	info->must_eat_count = -1;
// 	if (argc - 1 == 5)
// 		info->must_eat_count = ft_atoi(argv[5]);
// 	info->philos = make_philosophers(info);
// 	if (!info->philos)
// 		return (NULL);
// 	if (!setup_global_mutexes(info))
// 		return (NULL);
// 	info->sim_stop = 0;
// 	return (info);
// } 

// int start(int argc, char **argv)
// {
// 	t_info *info;
// 	// int valid = 1;
// 	if (argc < 5 || argc > 6)
// 	{
// 		printf("Wrong argument count!\n");
// 		return 1;
// 	}
// 	if (!is_valid_input(argc, argv))
// 		return 1;
// 	info = init_data(argc, argv);
// 	if (!info)
// 		return 1;
// 	if (!begin_simulation(info))
// 		return 1;
// 	end_simulation(info);
// 	return 0;
// }

// int main(int argc, char **argv)
// {
// 	int result = start(argc, argv);
// 	return result;
// }

#ifndef PHILO_H
# define PHILO_H

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

int is_simulation_stopped(t_info *info);
int start(int argc, char **argv);
int is_valid_input(int argc, char **argv);
int ft_atoi(char *str);
void *error_null(char *str, char *details, t_info *data);
int error_failure(char *str, char *details, t_info *data);
int msg(char *str, char *detail, int exit_no);
void destroy_mutexes(t_info *data);
void *free_table(t_info *data);
t_info *init_data(int ac, char **av);
void sim_start_delay(time_t start_time);
void p_sleep(t_info *data, time_t sleep_time);
time_t current_time(void);
void *philosopher(void *input);
void *monitor(void *src);
void print_outcome(t_info *data);
void write_status(t_philo *p, int report, t_status st);

#endif

// Memory cleanup function
void *free_table(t_info *data)
{
	unsigned int i;

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

// Destroy all mutexes
void destroy_mutexes(t_info *data)
{
	unsigned int i = 0;

	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->fork_locks[i]);
		pthread_mutex_destroy(&data->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->sim_stop_lock);
}

// Print message with formatting
int msg(char *str, char *detail, int exit_no)
{
	if (detail)
		printf(str, "philo :", detail);
	else
		printf(str, "philo :");
	return (exit_no);
}

// Handle errors and cleanup
int error_failure(char *str, char *details, t_info *data)
{
	if (data)
		free_table(data);
	return (msg(str, details, 0));
}

// Handle null errors and cleanup
void *error_null(char *str, char *details, t_info *data)
{
	if (data)
		free_table(data);
	msg(str, details, 1);
	return (NULL);
}

// Check if string contains only digits
int check_digits(char *str)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// Convert string to integer with overflow check
int ft_atoi(char *str)
{
	unsigned long long int nbr = 0;
	int i = 0;

	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (nbr > INT_MAX)
		return (-1);
	return ((int)nbr);
}

// Validate command line arguments
int is_valid_input(int ac, char **av)
{
	int i = 1;
	int num;

	while (i < ac)
	{
		num = ft_atoi(av[i]);
		if (!check_digits(av[i]) || (i != 1 && num == -1))
		{
			printf("🔢 Error : the input must be digits only 🔢\n");
			return (0);
		}
		if (i == 1 && (num <= 0 || num > 200))
		{
			printf("❌ you are not allowed to create this number of philosophers ❌\n");
			return (0);
		}
		i++;
	}
	return (1);
}

// Get current time in milliseconds
time_t current_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Sleep with simulation check
void p_sleep(t_info *data, time_t sleep_time)
{
	time_t wake_up = current_time() + sleep_time;

	while (current_time() < wake_up)
	{
		if (is_simulation_stopped(data))
			break;
		usleep(100);
	}
}

// Wait for simulation start
void sim_start_delay(time_t start_time)
{
	while (current_time() < start_time)
		continue;
}

// Eating and sleeping routine
void perform_eat_sleep(t_philo *p)
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

// Thinking routine
void do_think(t_philo *p, int quiet)
{
	time_t think_time;

	pthread_mutex_lock(&p->meal_time_lock);
	think_time = (p->data->time_to_die - (current_time() - p->last_meal) - p->data->time_to_eat) / 2;
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

// Single philosopher case
void *single_philo(void *arg)
{
	t_philo *p = (t_philo *)arg;

	pthread_mutex_lock(&p->data->fork_locks[p->fork[0]]);
	write_status(p, 0, TAKING_FORK_1);
	p_sleep(p->data, p->data->time_to_die);
	write_status(p, 0, DIED);
	pthread_mutex_unlock(&p->data->fork_locks[p->fork[0]]);
	return (NULL);
}

// Main philosopher routine
void *philosopher(void *input)
{
	t_philo *p = (t_philo *)input;

	if (p->data->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&p->meal_time_lock);
	p->last_meal = p->data->start_time;
	pthread_mutex_unlock(&p->meal_time_lock);
	sim_start_delay(p->data->start_time);
	if (p->data->time_to_die == 0)
		return (NULL);
	if (p->data->num_philos == 1)
		return (single_philo(p));
	else if (p->id % 2)
		do_think(p, 1);
	while (is_simulation_stopped(p->data) == 0)
	{
		perform_eat_sleep(p);
		do_think(p, 0);
	}
	return (NULL);
}

// Print status message
void status_print(t_philo *p, char *msg)
{
	printf("%ld %d %s\n", current_time() - p->data->start_time, p->id + 1, msg);
}

// Write philosopher status
void write_status(t_philo *p, int report, t_status st)
{
	pthread_mutex_lock(&p->data->write_lock);
	if (is_simulation_stopped(p->data) == 1 && report == 0)
	{
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

// Print final outcome
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
	printf("%d/%d philosophers ate at least %d times.\n", satisfied, info->num_philos, info->must_eat_count);
	pthread_mutex_unlock(&info->write_lock);
}

// Update simulation stop flag
void update_sim_stop(t_info *info, bool flag)
{
	pthread_mutex_lock(&info->sim_stop_lock);
	info->sim_stop = flag;
	pthread_mutex_unlock(&info->sim_stop_lock);
}

// Check if simulation is stopped
int is_simulation_stopped(t_info *info)
{
	int stopped = 0;

	pthread_mutex_lock(&info->sim_stop_lock);
	if (info->sim_stop == 1)
		stopped = 1;
	pthread_mutex_unlock(&info->sim_stop_lock);
	return (stopped);
}

// Check if philosopher died
int check_philo_death(t_philo *p)
{
	time_t now = current_time();

	if ((now - p->last_meal) >= p->data->time_to_die)
	{
		update_sim_stop(p->data, 1);
		write_status(p, 1, DIED);
		pthread_mutex_unlock(&p->meal_time_lock);
		return (1);
	}
	return (0);
}

// Check if simulation should end
int simulation_should_end(t_info *info)
{
	unsigned int idx = 0;
	int everyone_ate = 1;

	while (idx < info->num_philos)
	{
		pthread_mutex_lock(&info->philos[idx]->meal_time_lock);
		if (check_philo_death(info->philos[idx]))
			return (1);
		if (info->must_eat_count != -1)
			if (info->philos[idx]->times_ate < (unsigned int)info->must_eat_count)
				everyone_ate = 0;
		pthread_mutex_unlock(&info->philos[idx]->meal_time_lock);
		idx++;
	}
	if (info->must_eat_count != -1 && everyone_ate == 1)
	{
		update_sim_stop(info, 1);
		return (1);
	}
	return (0);
}

// Monitor thread function
void *monitor(void *src)
{
	t_info *info = (t_info *)src;

	if (info->must_eat_count == 0)
		return (NULL);
	update_sim_stop(info, 0);
	sim_start_delay(info->start_time);
	while (1)
	{
		if (simulation_should_end(info) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

// Start simulation
int begin_simulation(t_info *info)
{
	unsigned int idx = 0;

	info->start_time = current_time() + (info->num_philos * 2 * 10);
	while (idx < info->num_philos)
	{
		if (pthread_create(&info->philos[idx]->thread, NULL, &philosopher, info->philos[idx]) != 0)
			return (error_failure("Thread creation failed.\n", NULL, info));
		idx++;
	}
	if (info->num_philos > 1)
	{
		if (pthread_create(&info->grim_reaper, NULL, &monitor, info) != 0)
			return (error_failure("Thread creation failed.\n", NULL, info));
	}
	return (1);
}

// End simulation and cleanup
void end_simulation(t_info *info)
{
	unsigned int idx = 0;

	while (idx < info->num_philos)
	{
		pthread_join(info->philos[idx]->thread, NULL);
		idx++;
	}
	if (info->num_philos > 1)
		pthread_join(info->grim_reaper, NULL);
	if (info->must_eat_count != -1)
		print_outcome(info);
	destroy_mutexes(info);
	free_table(info);
}
static pthread_mutex_t	*create_fork_mutexes(t_info *info)
{
	pthread_mutex_t *mutexes;
	unsigned int idx = 0;
	mutexes = malloc(sizeof(pthread_mutex_t) * info->num_philos);
	if (!mutexes)
		return (error_null("Memory allocation failed for forks.\n", NULL, 0));
	while (idx < info->num_philos)
	{
		if (pthread_mutex_init(&mutexes[idx], 0) != 0)
			return (error_null("Mutex creation failed for fork.\n", NULL, 0));
		idx++;
	}
	return (mutexes);
}
//The ft_min/ft_max assignment is the key to preventing deadlock!
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
	t_philo **philos_arr;
	unsigned int idx = 0;
	philos_arr = malloc(sizeof(t_philo) * info->num_philos);
	if (!philos_arr)
		return (error_null("Philosopher allocation failed.\n", NULL, 0));
	while (idx < info->num_philos)
	{
		philos_arr[idx] = malloc(sizeof(t_philo));
		if (!philos_arr[idx])
			return (error_null("Philosopher allocation failed.\n", NULL, 0));
		if (pthread_mutex_init(&philos_arr[idx]->meal_time_lock, 0) != 0)
			return (error_null("Mutex creation failed for meal time.\n", NULL, 0));
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
		return (error_failure("Mutex creation failed.\n", NULL, info));
	if (pthread_mutex_init(&info->write_lock, 0) != 0)
		return (error_failure("Mutex creation failed.\n", NULL, info));
	return (1);
}

// Entry point for simulation data setup
t_info	*init_data(int argc, char **argv)
{
	t_info *info;
	info = malloc(sizeof(t_info));
	if (!info)
		return (error_null("Info allocation failed.\n", NULL, 0));
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

// Main start function
int start(int argc, char **argv)
{
	t_info *info;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong argument count!\n");
		return (1);
	}
	if (!is_valid_input(argc, argv))
		return (1);
	info = init_data(argc, argv);
	if (!info)
		return (1);
	if (!begin_simulation(info))
		return (1);
	end_simulation(info);
	return (0);
}

// Main function
int main(int argc, char **argv)
{
	int result = start(argc, argv);
	return (result);
}
