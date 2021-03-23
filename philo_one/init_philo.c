/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/24 00:40:16 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int					init_input_args(t_input_args *input_args, \
									char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		return (print_error(NUMBER_ARGS, NULL));
	input_args->amount_philo = ft_atoi(argv[1]);
	input_args->time_die = ft_atoi(argv[2]);
	input_args->time_eat = ft_atoi(argv[3]);
	input_args->time_sleep = ft_atoi(argv[4]);
	input_args->times_must_eat = argv[5] ? ft_atoi(argv[5]) : 0;
	if (input_args->amount_philo < 2 || input_args->amount_philo > 200 ||
		input_args->time_die < 60 || input_args->time_eat < 60 ||
		input_args->time_sleep < 60 || input_args->times_must_eat < 0)
		return (print_error(INCORRECT_ARGS, NULL));
	input_args->time_start = get_time();
	input_args->is_dead = 0;
	return (SUCCES);
}

static void			*ret_error(int err_f, t_philosopher_args *philo_args)
{
	if (err_f == 1)
		print_error(MALLOC_ERR, NULL);
	if (err_f == 2)
		print_error(MALLOC_ERR, philo_args);
	return (NULL);
}

static void			init_philo(t_philosopher_args *philo_args,
								pthread_mutex_t *forks, int num_ph, int i)
{
	philo_args[i].number_philo = i + 1;
	philo_args[i].left_fork = &forks[i];
	if (philo_args[i].number_philo == num_ph)
		philo_args[i].right_fork = &forks[0];
	else
		philo_args[i].right_fork = &forks[i + 1];
	philo_args[i].forks = forks;
}

t_philosopher_args	*init_philosopher_args(t_input_args *input_args)
{
	pthread_mutex_t		*forks;
	t_philosopher_args	*philo_args;
	int					i;

	if (!(philo_args = malloc(sizeof(t_philosopher_args) *
		input_args->amount_philo)))
		return (ret_error(1, NULL));
	if (!(forks = malloc(sizeof(pthread_mutex_t) * input_args->amount_philo)))
		return (ret_error(2, philo_args));
	i = -1;
	while (++i < input_args->amount_philo)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < input_args->amount_philo)
	{
		init_philo(philo_args, forks, input_args->amount_philo, i);
		philo_args[i].input_args = input_args;
		philo_args[i].last_meal = 0;
		pthread_mutex_init(&philo_args[i].checker_mutex, NULL);
	}
	return (philo_args);
}
