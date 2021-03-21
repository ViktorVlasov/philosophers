/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:55:08 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/21 21:12:46 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

int	ft_atoi(const char *str)
{
	int	nb;
	int	mark;

	mark = 0;
	nb = 0;
	while (*str == ' ' || *str == '\n' || *str == '\r' ||
		*str == '\v' || *str == '\f' || *str == '\t')
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			mark = 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nb = 10 * nb + (*str++ - '0');
	}
	if (mark == 1)
	{
		return (-1 * nb);
	}
	return (nb);
}

void free_all(t_philosopher_args *philo_args)
{
	int i;
	int ph_num;

	i = -1;
	ph_num = philo_args->input_args->amount_philo;
	if (philo_args)
	{
		while (++i < ph_num)
		{
			if (philo_args->forks)
				pthread_mutex_destroy(&philo_args->forks[i]);
			pthread_mutex_destroy(&philo_args[i].checker_mutex);
		}
		if (philo_args->forks)
			free(philo_args->forks);
		free(philo_args);
	}
}

int print_error(int code_error, t_philosopher_args *philo_args)
{
	if (code_error == NUMBER_ARGS)
	{
		printf("Invalid number of arguments! Requires five arguments: \n");
		printf("number_of_philosophers, time_to_die, time_to_eat");
		printf("time_to_sleep, number_of_times_each_philosopher_must_eat");
	}
	else if (code_error == INCORRECT_ARGS)
		printf("Incorrect arguments! Try again :)");
	else if (code_error == MALLOC_ERR)
		printf("Memory allocation error!");
	if (philo_args)
		free_all(philo_args);
	return (1);
}