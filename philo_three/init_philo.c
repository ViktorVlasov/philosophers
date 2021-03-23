/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/23 18:11:52 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

static void			*ret_error(int err_f, t_philosopher_args *philo_args)
{
	if (err_f == 1)
		print_error(MALLOC_ERR, NULL);
	if (err_f == 2)
		print_error(MALLOC_ERR, philo_args);
	return (NULL);
}

static void			init_sem_unlink(void)
{
	sem_unlink("forks");
	sem_unlink("checker");
	sem_unlink("fix");
}

t_philosopher_args	*init_philosopher_args(t_input_args *input_args)
{
	sem_t				*forks;
	t_philosopher_args	*philo_args;
	int					i;
	sem_t				*checker;
	sem_t				*fix;

	if (!(philo_args = malloc(sizeof(t_philosopher_args) *
		input_args->amount_philo)))
		return (ret_error(1, NULL));
	init_sem_unlink();
	forks = sem_open("forks", O_CREAT | O_EXCL, 0644, input_args->amount_philo);
	checker = sem_open("checker", O_CREAT | O_EXCL, 0644, 1);
	fix = sem_open("fix", O_CREAT | O_EXCL, 0644, 1);
	i = -1;
	while (++i < input_args->amount_philo)
	{
		philo_args[i].number_philo = i + 1;
		philo_args[i].forks = forks;
		philo_args[i].input_args = input_args;
		philo_args[i].last_meal = 0;
		philo_args[i].checker = checker;
		philo_args[i].fix = fix;
	}
	return (philo_args);
}
