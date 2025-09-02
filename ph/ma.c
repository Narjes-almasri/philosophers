
#include "philo.h"
#include "ph.h"

static t_philo	**initialize_philos(t_data *data)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!philos)
		return (error_null("%scant allocate memory.\n", NULL, 0));
	i = 0;
	while (i < data->num_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null("%scant allocate memory.\n", NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_lock, 0) != 0)
			return (error_null("%scant create mutex.\n", NULL, 0));
		philos[i]->table = data;
		philos[i]->id = i;
		philos[i]->eaten_times = 0;
		philos[i]->last_meal = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}


t_data	*init_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null("%s error: cant allocate memory.\n", NULL, 0));

	table->num_of_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->num_of_meals = -1;
	if (ac - 1 == 5)
		table->num_of_meals = ft_atoi(av[5]);
	table->philos = initialize_philos(table);
	if (!table->philos)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->stop_execution = 0;
	return (table);
}

int	is_valid_input(int ac, char **av)
{
	int	i;
	int	num;

	i = 1;
	while (i < ac)
	{
		num = ft_atoi(av[i]);
		// printf("num is %d\n", num);
		// printf("check_digits(av[i] is %d\n", check_digits(av[i]));
		if ((!check_digits(av[i])) || (i != 1 && num == -1))//why i != 1 because number of philosophers should not be -1 also the 1 (num of philo ) is checked below in the next if
		{
			printf("🔢 Error : the input must be digits only 🔢\n");
			return (0);
		}
		if (i == 1 && (num <= 0 || num > 200))//to check number of philosophers
		{
			printf("❌ you are not allowed to create this number of philosophers ❌\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int main(int ac,char **av)
{
	t_table	*table;

	if(ac < 5 || ac > 6)
	{
		printf("invalid number of arguments!!🩴\n");
		return (1);
	}
	if (!is_valid_input(ac, av))
		return (1);
	table = init_table(ac, av);
	if (!table)
		return (1);
	return (0);
}
