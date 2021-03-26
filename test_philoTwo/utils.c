/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:55:08 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/27 02:08:50 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int				ft_atoi(const char *str)
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

void			free_all(t_philosopher_args *philo_args)
{
	if (philo_args)
	{
		sem_close(philo_args->checker_sem);
		sem_close(philo_args->forks);
		sem_close(philo_args->sem_print);
		sem_close(philo_args->sem_block);
		sem_unlink("sem_print");
		sem_unlink("sem_block");
		sem_unlink("checker");
		sem_unlink("forks");
		free(philo_args);
	}
}

int				print_error(int code_error, t_philosopher_args *philo_args)
{
	if (code_error == NUMBER_ARGS)
	{
		printf("Invalid number of arguments! Requires five arguments: \n");
		printf("number_of_philosophers, time_to_die, time_to_eat\n");
		printf("time_to_sleep, number_of_times_each_philosopher_must_eat\n");
	}
	else if (code_error == INCORRECT_ARGS)
		printf("Incorrect arguments! Try again :)\n");
	else if (code_error == MALLOC_ERR)
		printf("Memory allocation error!\n");
	if (philo_args)
		free_all(philo_args);
	return (1);
}

unsigned int	get_time(void)
{
	struct timeval	tv;
	unsigned int	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}
