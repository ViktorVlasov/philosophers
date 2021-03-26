/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_twtwo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddraco <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/26 22:66:47 by ddraco           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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
			p_args->input_args->is_dead = 1;
			sem_post(p_args->checker_sem);
			return (NULL);
		}
		sem_post(p_args->checker_sem);
		usleep(MS);
	}
	return (NULL);
}

int		eat(t_philosopher_args *p_args, int *count_meal)
{
	if (p_args->input_args->is_dead)
		return (FAIL);
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
			count_meal == p_args->input_args->times_must_eat) ||
			p_args->input_args->is_dead)
			break ;
		print_mesg(p_args, MSG_SLEEP);
		if (p_args->input_args->is_dead)
			break ;
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	t_input_args		input_args;
	t_philosopher_args	*philo_args;
	pthread_t			*phil;
	int					i;

	if (init_input_args(&input_args, argv, argc) ||
		!(philo_args = init_philosopher_args(&input_args, -1)))
		return (FAIL);
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		return (print_error(MALLOC_ERR, philo_args));
	i = -1;
	while (++i < input_args.amount_philo)
	{
		pthread_create(&phil[i], NULL, philosophize, &philo_args[i]);
		usleep(MS);
	}
	i = -1;
	while (++i < input_args.amount_philo)
		pthread_join(phil[i], NULL);
	if (phil)
		free(phil);
	free_all(philo_args);
}
