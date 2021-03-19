/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/19 16:54:24 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

// Что, если times_must_eat == 0?

void init_input_args(t_input_args *input_args, char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		exit(1);
	input_args->amount_philo = atoi(argv[1]);
	input_args->time_die = atoi(argv[2]); 
	input_args->time_eat = atoi(argv[3]);
	input_args->time_sleep = atoi(argv[4]);
	input_args->times_must_eat = argv[5] ? atoi(argv[5]) : 0;
}


void init_philosopher(t_philosopher *philosopher, unsigned int number_philo, \
						unsigned int left_fork, unsigned int right_fork) {
    philosopher->number_philo = number_philo;
    philosopher->left_fork = left_fork;
    philosopher->right_fork = right_fork;
}

t_philosopher_args *init_philosopher_args(t_input_args *input_args)
{
	int i;
	pthread_mutex_t *forks;
	t_philosopher_args *philo_args;
	pthread_mutex_t entry_point;
	
	if (!(philo_args = malloc(sizeof(t_philosopher_args) * input_args->amount_philo)))
		exit(1);
	if (!(forks = malloc(sizeof(pthread_mutex_t) * input_args->amount_philo)))
		exit(1);
	// Получили массив из num_philo элементов
    
	i = -1;
	while (++i < input_args->amount_philo)
	    pthread_mutex_init(&forks[i], NULL);
	pthread_mutex_init(&entry_point, NULL);
	i = -1;
	while (++i < input_args->amount_philo)
	{
		if (i == (input_args->amount_philo - 1))
			init_philosopher(&philo_args[i].philosopher, i + 1, i, 0);
		else
			init_philosopher(&philo_args[i].philosopher, i + 1, i, i + 1);
		philo_args[i].forks = forks;
		philo_args[i].entry_point = &entry_point;
		philo_args[i].input_args = input_args;
	}
	return (philo_args);
}

void print_mesg(t_philosopher_args *p_args, char *mesg)
{
	printf("%d %s", p_args->philosopher.number_philo, mesg);	
}

void *eat(void *args)
{
	t_philosopher_args *p_args;
	t_philosopher philo;
	pthread_mutex_t *forks;
	int count_eat;
	unsigned int max_eat;
	
	p_args = (t_philosopher_args*)args;
	philo = p_args->philosopher;
	forks = p_args->forks;

	printf("%d", p_args->input_args->amount_philo);
	// max_eat = p_args->input_args->times_must_eat;
	// count_eat = 0;

	// printf("%d is eating\n", p_args->philosopher.number_philo);
	// while (1)
	// {	
	// 	pthread_mutex_lock(&forks[philo.left_fork]);
	// 	pthread_mutex_lock(&forks[philo.right_fork]);
	// 	pthread_mutex_lock(p_args->entry_point);
	// 	print_mesg(p_args, MESG_FORK);
	// 	print_mesg(p_args, MESG_FORK);
	// 	pthread_mutex_unlock(p_args->entry_point);
		
	// 	pthread_mutex_lock(p_args->entry_point);
	// 	usleep(p_args->input_args->time_eat * MS);
	// 	print_mesg(p_args, MESG_EAT);
	// 	//philo->last_meal = get_time(s->data->t_start_usec, s->data->t_start_sec);
	// 	pthread_mutex_unlock(p_args->entry_point);
	// 	pthread_mutex_unlock(&forks[philo.right_fork]);
	// 	pthread_mutex_unlock(&forks[philo.left_fork]);
		
	// 	if (max_eat != 0 && count_eat == max_eat)
	// 		break;
	// 	count_eat += 1;
	// }
}

int main(int argc, char **argv)
{
	t_input_args input_args;
	t_philosopher_args *philo_args;
	pthread_t *phil;
	/* Init args, mutex(forks) and philosophers */

	init_input_args(&input_args, argv, argc);
	philo_args = init_philosopher_args(&input_args);		
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		exit(1);

	printf("%d, %d, %d, %d, %d\n", input_args.amount_philo, input_args.time_die, input_args.time_eat,
								input_args.time_sleep, input_args.times_must_eat);
	
	int i = -1;
	while (++i < 5)
		pthread_create(&phil[i], NULL, eat, &philo_args[i]);
	i = -1;
	while (++i < 5)
		pthread_join(phil[i], NULL);
	
}
