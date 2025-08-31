/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:55:11 by naalmasr          #+#    #+#             */
/*   Updated: 2025/08/31 18:45:00 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static int	check_digits(char *str)
{
	int	i;

	i = 0;
	// printf("str is %s\n", str);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str)
{
	unsigned long long int	nbr;
	int						i;

	i = 0;
	nbr = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (nbr > INT_MAX)
		return (-1);
	return ((int)nbr);
}

int	is_correct_input(int ac, char **av)
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
