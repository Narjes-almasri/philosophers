/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:04:22 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 10:16:26 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	check_digits(char *str)
{
	int	i;

	i = 0;
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

int	is_valid_input(int ac, char **av)
{
	int	i;
	int	num;

	i = 1;
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
			printf("❌ you are not allowed to create this number of philos❌\n");
			return (0);
		}
		i++;
	}
	return (1);
}
