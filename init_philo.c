/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/20 20:09:29 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

# define NUMBER_ARGS 1
# define INCORRECT_ARGS 2
# define MALLOC_ERR 3

void print_error(int code_error)
{
	if (code_error == NUMBER_ARGS)
		printf("Invalid number of arguments! Requires five arguments: \n\
				number_of_philosophers, time_to_die, time_to_eat \
				time_to_sleep, number_of_times_each_philosopher_must_eat");
	else if (code_error == INCORRECT_ARGS)
		printf("Incorrect arguments! Try again :)");
	else if (code_error == MALLOC_ERR)
		printf("Memory allocation error!");
	exit(FAIL);
}

void init_input_args(t_input_args *input_args, char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		print_error(NUMBER_ARGS);
	input_args->amount_philo = ft_atoi(argv[1]);
	input_args->time_die = ft_atoi(argv[2]); 
	input_args->time_eat = ft_atoi(argv[3]);
	input_args->time_sleep = ft_atoi(argv[4]);
	input_args->times_must_eat = argv[5] ? ft_atoi(argv[5]) : 0;
	if (input_args->amount_philo < 2 || input_args->amount_philo > 200 || 
		input_args->time_die < 60 || input_args->time_eat < 60 ||
		input_args->time_sleep < 60 || input_args->times_must_eat < 0)
		print_error(INCORRECT_ARGS);
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
		print_error(MALLOC_ERR);
	if (!(forks = malloc(sizeof(pthread_mutex_t) * input_args->amount_philo)))
		print_error(MALLOC_ERR);
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
		philo_args[i].time_sec = time_sec;
		philo_args[i].time_usec = time_usec;
	}
	return (philo_args);
}
