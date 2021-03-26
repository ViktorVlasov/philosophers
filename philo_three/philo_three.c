/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 01:27:01 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/27 01:59:00 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	print_mesg(t_philosopher_args *p_args, char *mesg)
{
	unsigned int t_start;

	t_start = p_args->input_args->time_start;
	sem_wait(p_args->sem_print);
	if (p_args->input_args->is_dead)
	{
		sem_post(p_args->sem_print);
		return ;
	}
	printf("%u %d %s\n", get_time() - t_start, \
		p_args->number_philo, mesg);
	sem_post(p_args->sem_print);
}

void	*check_death(void *args)
{
	t_philosopher_args *p_args;

	p_args = (t_philosopher_args*)args;
	pthread_detach(p_args->checker_thread);
	while (!p_args->input_args->is_dead)
	{
		sem_wait(p_args->checker_sem);
		if (!p_args->input_args->is_dead && get_time() > p_args->last_meal)
		{
			print_mesg(p_args, MSG_DIED);
			sem_wait(p_args->sem_print);
			p_args->input_args->is_dead = 1;
			exit(DIE);
		}
		sem_post(p_args->checker_sem);
		usleep(MS);
	}
	return (NULL);
}

int		eat(t_philosopher_args *p_args, int *count_meal)
{
	if (p_args->input_args->is_dead)
		exit(DIE);
	sem_wait(p_args->sem_block);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_post(p_args->sem_block);
	sem_wait(p_args->checker_sem);
	print_mesg(p_args, MSG_EAT);
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	sem_post(p_args->checker_sem);
	(*count_meal)++;
	usleep(p_args->input_args->time_eat * MS);
	sem_post(p_args->forks);
	sem_post(p_args->forks);
	return (SUCCES);
}

void	*philosophize(void *args)
{
	t_philosopher_args	*p_args;
	int					count_meal;

	p_args = (t_philosopher_args*)args;
	count_meal = 0;
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_create(&p_args->checker_thread, NULL, check_death, p_args);
	while (!p_args->input_args->is_dead)
	{
		eat(p_args, &count_meal);
		if ((p_args->input_args->times_must_eat != 0 &&
			count_meal == p_args->input_args->times_must_eat))
			exit(SUCCES);
		if (p_args->input_args->is_dead)
			exit(DIE);
		print_mesg(p_args, MSG_SLEEP);
		if (p_args->input_args->is_dead)
			exit(DIE);
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
	}
}

int		main(int argc, char **argv)
{
	t_input_args		input_args;
	t_philosopher_args	*philo_args;

	if (init_input_args(&input_args, argv, argc) ||
		!(philo_args = init_philosopher_args(&input_args, -1)))
		return (FAIL);
	if (start_p(philo_args, input_args) == FAIL)
		return (FAIL);
	free_all(philo_args);
	exit(SUCCES);
}
