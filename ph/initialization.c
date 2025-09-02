#include "philo.h"

// Creates and sets up mutexes for each fork.
static pthread_mutex_t	*create_fork_mutexes(t_data *table)
{
    pthread_mutex_t	*mutex_array;
    unsigned int	idx;

    mutex_array = malloc(sizeof(pthread_mutex_t) * table->num_philos);
    if (!mutex_array)
        return (error_null("Memory allocation failed for fork mutexes.\n", NULL, 0));
    idx = 0;
    while (idx < table->num_philos)
    {
        if (pthread_mutex_init(&mutex_array[idx], NULL) != 0)
            return (error_null("Unable to initialize fork mutex.\n", NULL, 0));
        idx++;
    }
    return (mutex_array);
}

// Assigns fork indices to each philosopher to avoid deadlock.
static void	set_philo_forks(t_philo *philo)
{
    philo->fork[0] = philo->id;
    philo->fork[1] = (philo->id + 1) % philo->data->num_philos;
    if (philo->id % 2 != 0)
    {
        philo->fork[0] = (philo->id + 1) % philo->data->num_philos;
        philo->fork[1] = philo->id;
    }
}

// Allocates and initializes philosopher structures.
static t_philo	**setup_philosophers(t_data *table)
{
    t_philo			**philo_list;
    unsigned int	j;

    philo_list = malloc(sizeof(t_philo *) * table->num_philos);
    if (!philo_list)
        return (error_null("Failed to allocate memory for philosophers.\n", NULL, 0));
    j = 0;
    while (j < table->num_philos)
    {
        philo_list[j] = malloc(sizeof(t_philo));
        if (!philo_list[j])
            return (error_null("Failed to allocate memory for philosopher struct.\n", NULL, 0));
        if (pthread_mutex_init(&philo_list[j]->meal_time_lock, NULL) != 0)
            return (error_null("Failed to initialize meal_time mutex.\n", NULL, 0));
        philo_list[j]->data = table;
        philo_list[j]->id = j;
        philo_list[j]->times_ate = 0;
        philo_list[j]->last_meal = 0;
        set_philo_forks(philo_list[j]);
        j++;
    }
    return (philo_list);
}

// Initializes all global mutexes for the simulation.
static int	setup_global_mutexes(t_data *table)
{
    table->fork_locks = create_fork_mutexes(table);
    if (!table->fork_locks)
        return (0);
    if (pthread_mutex_init(&table->sim_stop_lock, NULL) != 0)
        return (error_failure("Failed to initialize sim_stop mutex.\n", NULL, table));
    if (pthread_mutex_init(&table->write_lock, NULL) != 0)
        return (error_failure("Failed to initialize write mutex.\n", NULL, table));
    return (1);
}

// Prepares the main simulation data structure.
t_data	*prepare_table(int argc, char **argv)
{
    t_data	*table;

    table = malloc(sizeof(t_data));
    if (!table)
        return (error_null("Memory allocation failed for table struct.\n", NULL, 0));
    table->num_philos = ft_atoi(argv[1]);
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
    table->time_to_sleep = ft_atoi(argv[4]);
    table->must_eat_count = -1;
    if (argc == 6)
        table->must_eat_count = ft_atoi(argv[5]);
    table->philos = setup_philosophers(table);
    if (!table->philos)
        return (NULL);
    if (!setup_global_mutexes(table))
        return (NULL);
    table->sim_stop = 0;
    return (table);
}