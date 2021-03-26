/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_p.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 01:24:00 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/27 01:32:34 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	kill_process(t_philosopher_args *philo_args)
{
	int i;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
		kill(philo_args[i].pid_id, SIGKILL);
}

int		wait_process(t_philosopher_args *philo_args)
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
			if (WEXITSTATUS(status) == DIE)
				kill_process(philo_args);
		}
	}
	return (SUCCES);
}

int		start_p(t_philosopher_args *philo_args, t_input_args input_args)
{
	int i;

	i = -1;
	while (++i < input_args.amount_philo)
	{
		if (!(philo_args[i].pid_id = fork()))
			philosophize(&philo_args[i]);
		else if (philo_args[i].pid_id < 0)
		{
			kill_process(philo_args);
			free_all(philo_args);
			return (FAIL);
		}
		usleep(MS);
	}
	if (wait_process(philo_args) == FAIL)
	{
		free_all(philo_args);
		return (FAIL);
	}
	return (SUCCES);
}
