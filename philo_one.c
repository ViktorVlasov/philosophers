/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/17 14:06:13 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"


pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;


// void* eat(void *args) 
// {
//     t_philosopher_args *arg = (t_philosopher_args*)args;
//     const t_philosopher *philosopher = arg->philosopher;
//     const t_table *table = arg->table;
 
//     printf("%u started dinner\n", philosopher->number_philo);
 
	// pthread_mutex_lock(&table->forks[philosopher->left_fork]);
	// usleep(1000);
//     pthread_mutex_lock(&table->forks[philosopher->right_fork]);
 
//     printf("%u is eating\n", philosopher->number_philo);
 
//     pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
//     pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
 
//     printf("%u finished dinner\n", philosopher->number_philo);
// }

/*
void* eat(void *args) 
{
    t_philosopher_args *arg = (t_philosopher_args*)args;
    const t_philosopher *philosopher = arg->philosopher;
    const t_table *table = arg->table;
	//const t_input_args *input_args = arg->input_args;
    unsigned r;  
 
	// int cur_time;
	int times_eat = 0;
	if (input_args == 0)
	{
		while (1)
		{
			// философ ест + защита от дедлока
			pthread_mutex_lock(&entry_point);
			pthread_mutex_lock(&table->forks[philosopher->left_fork]);
			printf("%u is eating\n", philosopher->number_philo);
			usleep(input_args->time_eat);
			pthread_mutex_lock(&table->forks[philosopher->right_fork]);
			pthread_mutex_unlock(&entry_point);
			pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
			pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
			
			// философ спит
			printf("%u is sleeping\n", philosopher->number_philo);
			usleep(input_args->time_sleep);
			
			printf("%u is thinking\n", philosopher->number_philo);
					
			//printf("%u finished dinner\n", philosopher->number_philo);
		}
	}
} */


void *eat(void *args)
{
	printf("start eating\n");
}

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

int init_philosopher_args(t_philosopher_args *philo_args, 
							t_input_args *input_args)
{
	int i;
	pthread_mutex_t *forks;
	
	if (!(philo_args = malloc(sizeof(t_philosopher_args) * input_args->amount_philo)))
		exit(1);
	if (!(forks = malloc(sizeof(pthread_mutex_t) * input_args->amount_philo)))
		exit(1);
	// Получили массив из num_philo элементов
    
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
	}
	
}

int main(int argc, char **argv)
{
	t_input_args input_args;
	t_philosopher_args *philo_args;
	pthread_t *phil;
	/* Init args, mutex(forks) and philosophers */

	init_input_args(&input_args, argv, argc);
	init_philosopher_args(philo_args, &input_args);	
	if (!(phil = malloc(sizeof(pthread_t) * 5)))
		exit(1);

	printf("%d, %d, %d, %d, %d\n", input_args.amount_philo, input_args.time_die, input_args.time_eat,
								input_args.time_sleep, input_args.times_must_eat);
	
	int i = -1;
	while (++i < 5)
		pthread_create(&phil[i], NULL, eat, NULL);
	
	i = -1;
	while (++i < 5)
		pthread_join(phil[i], NULL);
}

/*
int main(int agrc, char **argv) 
{
    pthread_t *threads; // Массив потоков размера num_philo 
    t_philosopher *philosophers; // Массив философов
    t_philosopher_args *arguments; // Массив содержащий стол (массив вилок), который общий для всех и отдельного философа. Для передачи в поток
    t_table table; // Структура стола (содержит массив вилок)

    //size_t i;
	
	init_input_args(&input_args, argv);
 
	// Инициализация массива вилок с помощью мьютекса
	init_table(&table);
	
	// выделение памяти под массивы
	threads = malloc(sizeof(pthread_t) * (input_args.num_philo));
	philosophers = malloc(sizeof(t_philosopher) * (input_args.num_philo));
	arguments = malloc(sizeof(t_philosopher_args) * (input_args.num_philo));
		
	// Инициализация массива философов
	int num_philo = 0;
	while (num_philo < input_args.num_philo - 1)
	{
		init_philosopher(&philosophers[num_philo], num_philo, num_philo, num_philo + 1);
		num_philo++;
	}
	init_philosopher(&philosophers[input_args.num_philo - 1], input_args.num_philo - 1, input_args.num_philo - 1, 0);

	// for (int num_philo = 0; num_philo < input_args.num_philo - 1; num_philo++)
    // 	init_philosopher(&philosophers[num_philo], num_philo, num_philo, num_philo + 1);
	// init_philosopher(&philosophers[input_args.num_philo - 1], input_args.num_philo - 1, input_args.num_philo - 1, 0);
    // init_philosopher(&philosophers[1], "Bob", 1, 2);
   
	int i;
	i = 0;
	while (i < input_args.num_philo)
	{
		arguments[i].philosopher = &philosophers[i];
        arguments[i].table = &table;
		arguments[i].input_args = &input_args;
		i++;
	}

	i = 0;
	while (i < input_args.num_philo)
	{
		pthread_create(&threads[i], NULL, eat, &arguments[i]);
		i++;
	}
	
	i = 0;
	while (i < input_args.num_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	
	
    // for (i = 0; i < PHT_SIZE; i++) {
    //     arguments[i].philosopher = &philosophers[i];
    //     arguments[i].table = &table;
    // }
 
    // for (i = 0; i < PHT_SIZE; i++)
    //     pthread_create(&threads[i], NULL, eat, &arguments[i]);
		
    // for (i = 0; i < PHT_SIZE; i++)
    //     pthread_join(threads[i], NULL);


	// освобождение памяти 
    wait(NULL);
}
*/