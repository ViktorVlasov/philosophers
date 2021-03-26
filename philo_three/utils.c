/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddraco <efumarvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:55:08 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/26 22:02:43 by ddraco           ###   ########.fr       */
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
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			mark = 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		nb = 10 * nb + (*str++ - '0');
	if (mark == 1)
		return (-1 * nb);
	return (nb);
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

unsigned long				get_time(void)
{
	unsigned long			to_return;
	static struct timeval	time;

	gettimeofday(&time, NULL);
	to_return = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (to_return);
}

void			kill_process(t_philosopher_args *philo_args)
{
	int i;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
		kill(philo_args[i].pid_id, SIGKILL);
}

int				wait_process(t_philosopher_args *philo_args)
{
	int i;
	int status;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
	{
		if (wait(&status) == -1)
			return (FAIL);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == DIED)
				kill_process(philo_args);		
		}
	}
	return (SUCCES);
}
