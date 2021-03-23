/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:55:08 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/23 21:09:14 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

int				print_error(int code_error, t_philosopher_args *philo_args)
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
		free_all(philo_args, NULL);
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

void kill_process(t_philosopher_args *philo_args)
{
	(void)philo_args;
	// int i;

	// i = -1;
	//while (++i < philo_args->input_args->amount_philo)
	//{
	//	if (philo_args->pid_id > 0)
	//kill();
	kill(-1, SIGTERM);
	//}
}

int wait_process(t_philosopher_args *philo_args)
{
	int i;
	int status;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
	{
		if (wait(&status) == -1)
			return (FAIL);
		if (WIFEXITED(status) != 0)
			kill_process(philo_args);	
	}
	return (SUCCES);
}
