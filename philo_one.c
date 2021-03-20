/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/20 19:36:14 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

// Что, если times_must_eat == 0?
pthread_mutex_t g_entry_point;
pthread_mutex_t g_print;

void print_mesg(t_philosopher_args *p_args, char *mesg)
{
	printf("%d %s\n", p_args->philosopher.number_philo, mesg);	
}

void *check_death(void *args)
{
	unsigned int	cur_time;

	t_philosopher_args *p_args;
	
	p_args = (t_philosopher_args*)args;
	
	usleep(p_args->input_args->time_die * MS);
	cur_time = get_time(p_args->time_usec, p_args->time_sec);
	if (cur_time - philo->last_meal >= data->t_die)
	{
		pthread_mutex_lock(philo->m_display);
		data->one_die = TRUE;
		display_manager(s, philo, EVENT_DEAD);
	}
}

void *eat(void *args)
{
	t_philosopher_args *p_args;
	t_philosopher philo;
	pthread_mutex_t *forks;
	pthread_t checker;
	int count_eat;
	unsigned int max_eat;
	
	p_args = (t_philosopher_args*)args;
	philo = p_args->philosopher;
	forks = p_args->forks;

	max_eat = p_args->input_args->times_must_eat;
	count_eat = 0;

	while (1)
	{	
		pthread_detach(checker);
		pthread_create(&checker, NULL, &check_death, p_args);

		// Начинает брать вилки и есть
		pthread_mutex_lock(&g_entry_point); 
		pthread_mutex_lock(&forks[philo.left_fork]);
		print_mesg(p_args, MSG_FORK);
		pthread_mutex_lock(&forks[philo.right_fork]);
		print_mesg(p_args, MSG_FORK);
		pthread_mutex_unlock(&g_entry_point);
		
		print_mesg(p_args, MSG_EAT);
		usleep(p_args->input_args->time_eat * MS);
		// //philo->last_meal = get_time(s->data->t_start_usec, s->data->t_start_sec);
		pthread_mutex_unlock(&forks[philo.right_fork]);
		pthread_mutex_unlock(&forks[philo.left_fork]);
		// Наелся

		pthread_mutex_lock(&g_entry_point);
		print_mesg(p_args, MSG_SLEEP);
		pthread_mutex_unlock(&g_entry_point);
		usleep(p_args->input_args->time_sleep * MS);

		pthread_mutex_lock(&g_entry_point);
		print_mesg(p_args, MSG_THINK);
		pthread_mutex_unlock(&g_entry_point);
		if (max_eat != 0 && count_eat == max_eat)
			break ;
		count_eat += 1;
	}
}

int main(int argc, char **argv)
{
	t_input_args input_args;
	t_philosopher_args *philo_args;
	pthread_t *phil;

	pthread_mutex_init(&g_entry_point, NULL);
	pthread_mutex_init(&g_print, NULL);
	init_input_args(&input_args, argv, argc);
	philo_args = init_philosopher_args(&input_args);		
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		exit(1);

	// printf("%d, %d, %d, %d, %d\n", input_args.amount_philo, input_args.time_die, input_args.time_eat,
	// 							input_args.time_sleep, input_args.times_must_eat);
	
	int i = -1;
	while (++i < input_args.amount_philo)
		pthread_create(&phil[i], NULL, eat, &philo_args[i]);
	i = -1;
	while (++i < input_args.amount_philo)
		pthread_join(phil[i], NULL);
	
}
