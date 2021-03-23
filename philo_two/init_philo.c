/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/23 11:32:08 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void				init_input_args(t_input_args *input_args, \
									char **argv, int argc)
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

void				init_philosopher(t_philosopher *philosopher,\
									unsigned int number_philo,\
									unsigned int left_fork,\
									unsigned int right_fork)
{
	philosopher->number_philo = number_philo;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = right_fork;
}

static void			*ret_error(int err_f, t_philosopher_args *philo_args)
{
	if (err_f == 1)
		print_error(MALLOC_ERR, NULL);
	if (err_f == 2)
		print_error(MALLOC_ERR, philo_args);
	return (NULL);
}

t_philosopher_args	*init_philosopher_args(t_input_args *input_args)
{
	sem_t				*forks;
	t_philosopher_args	*philo_args;
	int					i;
	sem_t				*checker_mutex;

	if (!(philo_args = malloc(sizeof(t_philosopher_args) *
		input_args->amount_philo)))
		return (ret_error(1, NULL));
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT | O_EXCL, 0644, input_args->amount_philo);
	sem_unlink("checker");
	checker_mutex = sem_open("checker", O_CREAT | O_EXCL, 0644, 1);
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
		philo_args[i].checker_mutex = checker_mutex;
	}
	return (philo_args);
}
