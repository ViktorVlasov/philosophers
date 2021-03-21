/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/21 21:25:16 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

// Что, если times_must_eat == 0?
pthread_mutex_t g_entry_point;
pthread_mutex_t g_print;

void print_mesg(t_philosopher_args *p_args, char *mesg)
{
	unsigned int t_start;

	t_start = p_args->input_args->time_start;
	pthread_mutex_lock(&g_print);
	if (p_args->input_args->is_dead)
	{
		pthread_mutex_unlock(&g_print);
		return ;
	}	
	printf("%u %d %s\n", get_time() - t_start, p_args->philosopher.number_philo, mesg);	
	pthread_mutex_unlock(&g_print);
}

void *check_death(void *args)
{
	t_philosopher_args *p_args;
	
	p_args = (t_philosopher_args*)args;
	pthread_detach(p_args->checker_thread);
	while (!p_args->input_args->is_dead)
	{
		pthread_mutex_lock(&p_args->checker_mutex);
		if (!p_args->input_args->is_dead && get_time() > p_args->last_meal)
		{
			print_mesg(p_args, MSG_DIED);
			p_args->input_args->is_dead = 1;
			pthread_mutex_unlock(&p_args->checker_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&p_args->checker_mutex);
		usleep(MS); 
	}
	return (NULL);
}

int eat(t_philosopher_args *p_args, t_philosopher philo, int *count_meal)
{
	if (p_args->input_args->is_dead)
		return (FAIL);
	pthread_mutex_lock(&p_args->forks[philo.left_fork]);
	print_mesg(p_args, MSG_FORK);
	pthread_mutex_lock(&p_args->forks[philo.right_fork]);
	print_mesg(p_args, MSG_FORK);
	pthread_mutex_lock(&p_args->checker_mutex);
	print_mesg(p_args, MSG_EAT);
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_mutex_unlock(&p_args->checker_mutex);
	(*count_meal)++;
	usleep(p_args->input_args->time_eat * MS);
	pthread_mutex_unlock(&p_args->forks[philo.right_fork]);
	pthread_mutex_unlock(&p_args->forks[philo.left_fork]);
}

void *philosophize(void *args)
{
	t_philosopher_args *p_args;
	t_philosopher philo;
	pthread_t checker;
	int count_meal;
	
	p_args = (t_philosopher_args*)args;
	philo = p_args->philosopher;
	count_meal = 0;
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_create(&p_args->checker_thread, NULL, check_death, p_args);
	if (philo.number_philo % 2 == 0)
		usleep(MS * p_args->input_args->time_eat);
	while (!p_args->input_args->is_dead)
	{
		eat(p_args, philo, &count_meal);
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
	pthread_mutex_unlock(&p_args->forks[philo.right_fork]);
	pthread_mutex_unlock(&p_args->forks[philo.left_fork]);
	return (NULL);
}

int main(int argc, char **argv)
{
	t_input_args input_args;
	t_philosopher_args *philo_args;
	pthread_t *phil;
	int i;

	pthread_mutex_init(&g_print, NULL);
	init_input_args(&input_args, argv, argc);
	if (!(philo_args = init_philosopher_args(&input_args)))
		return (1);
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		return (print_error(MALLOC_ERR, philo_args));
	i = -1;
	while (++i < input_args.amount_philo)
		pthread_create(&phil[i], NULL, philosophize, &philo_args[i]);
	i = -1;
	while (++i < input_args.amount_philo)
		pthread_join(phil[i], NULL);
	pthread_mutex_destroy(&g_print);
	if (phil)
		free(phil);
	free_all(philo_args);
}
