/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/26 22:34:111 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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

void unlink_sems(void)
{
	sem_unlink("forks");
	sem_unlink("checker");
	sem_unlink("sem_block");
	sem_unlink("sem_print");
}

void init_philo(t_philosopher_args *philo_args, int i, 
				t_input_args *input_args)
{
	philo_args[i].number_philo = i + 1;
	philo_args[i].last_meal = 0;
	philo_args[i].input_args = input_args;
}

t_philosopher_args	*init_philosopher_args(t_input_args *input_args, int i)
{
	sem_t				*forks;
	sem_t				*checker_sem;
	sem_t				*sem_print;
	sem_t				*sem_block;
	t_philosopher_args	*philo_args;

	if (!(philo_args = malloc(sizeof(t_philosopher_args) *
		input_args->amount_philo)))
		return (ret_error(1, NULL));
	unlink_sems();
	forks = sem_open("forks", O_CREAT | O_EXCL, 0644, input_args->amount_philo);
	checker_sem = sem_open("checker", O_CREAT | O_EXCL, 0644, 1);
	sem_block = sem_open("sem_block", O_CREAT | O_EXCL, 0644, 1);
	sem_print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 1);
	while (++i < input_args->amount_philo)
	{
		init_philo(philo_args, i, input_args);
		philo_args[i].forks = forks;
		philo_args[i].checker_sem = checker_sem;
		philo_args[i].sem_block = sem_block;
		philo_args[i].sem_print = sem_print;
	}
	return (philo_args);
}
