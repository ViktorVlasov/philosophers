/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/21 21:26:19 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

void init_input_args(t_input_args *input_args, char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		print_error(NUMBER_ARGS, NULL);
	input_args->amount_philo = ft_atoi(argv[1]);
	input_args->time_die = ft_atoi(argv[2]); 
	input_args->time_eat = ft_atoi(argv[3]);
	input_args->time_sleep = ft_atoi(argv[4]);
	input_args->times_must_eat = argv[5] ? ft_atoi(argv[5]) : 0;
	if (input_args->amount_philo < 2 || input_args->amount_philo > 200 || 
		input_args->time_die < 60 || input_args->time_eat < 60 ||
		input_args->time_sleep < 60 || input_args->times_must_eat < 0)
		print_error(INCORRECT_ARGS, NULL);
	input_args->time_start = get_time();
	input_args->is_dead = 0;
}


void init_philosopher(t_philosopher *philosopher, unsigned int number_philo, \
						unsigned int left_fork, unsigned int right_fork) {
	philosopher->number_philo = number_philo;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = right_fork;
}

t_philosopher_args *init_philosopher_args(t_input_args *input_args)
{
	pthread_mutex_t *forks;
	t_philosopher_args *philo_args;
	int i;
	
	if (!(philo_args = malloc(sizeof(t_philosopher_args) * input_args->amount_philo)))
		print_error(MALLOC_ERR, NULL);
	if (!(forks = malloc(sizeof(pthread_mutex_t) * input_args->amount_philo)))
		print_error(MALLOC_ERR, philo_args);
	i = -1;
	while (++i < input_args->amount_philo)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < input_args->amount_philo)
	{
		if (i == (input_args->amount_philo - 1))
			init_philosopher(&philo_args[i].philosopher, i + 1, i, 0);
		else
			init_philosopher(&philo_args[i].philosopher, i + 1, i, i + 1);
		philo_args[i].forks = forks;
		philo_args[i].input_args = input_args;
		philo_args[i].last_meal = 0;
		pthread_mutex_init(&philo_args[i].checker_mutex, NULL);
	}
	return (philo_args);
}
