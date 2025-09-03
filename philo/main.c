/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naalmasr <naalmasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:18:00 by naalmasr          #+#    #+#             */
/*   Updated: 2025/09/03 11:00:13 by naalmasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	start(int argc, char **argv)
{
	t_info	*info;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong num of arguments 🩴​🩴​!\n");
		return (1);
	}
	if (!is_valid_input(argc, argv))
		return (1);
	info = init_data(argc, argv);
	if (!info)
		return (1);
	if (!begin_sim(info))
		return (1);
	end_sim(info);
	return (0);
}

int	main(int argc, char **argv)
{
	int	result;

	result = start(argc, argv);
	return (result);
}
