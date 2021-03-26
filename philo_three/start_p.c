/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_p.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 02:04:09 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/25 02:27:17 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	*philosophize(void *args)
{
	t_philosopher_args	*p_args;
	int					max_eat;

	p_args = (t_philosopher_args*)args;
	max_eat = p_args->input_args->times_must_eat;
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_create(&p_args->checker_thread, NULL, check_death, p_args);
	if (p_args->number_philo % 2 == 0)
		usleep(p_args->input_args->time_eat * MS);
	while (!p_args->input_args->is_dead)
	{
		eat(p_args);
		if (p_args->input_args->is_dead || (p_args->input_args->times_must_eat &&\
		p_args->count_meal == p_args->input_args->times_must_eat))
		    break ;
        print_mesg(p_args, MSG_SLEEP);
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
	}
    if (p_args->input_args->is_dead)
        exit(DIED);
	exit(DIED);
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
	}
	if (wait_process(philo_args) == FAIL)
	{
		free_all(philo_args);
		return (FAIL);
	}
	return (SUCCES);
}