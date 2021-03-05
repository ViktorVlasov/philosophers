/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/05 16:22:42 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PHT_SIZE 5

typedef struct	s_input_args
{
	unsigned int num_philo;
	unsigned int time_die; 
	unsigned int time_eat;
	unsigned int time_sleep;
	unsigned int times_must_eat;
}				t_input_args;

typedef struct	s_philosopher {
	unsigned int number_philo;
	unsigned int left_fork;
	unsigned int right_fork;
}				t_philosopher;

typedef struct	s_table {
    pthread_mutex_t forks[PHT_SIZE];
} 				t_table;

typedef struct	s_philosopher_args {
    const t_philosopher *philosopher;
    const t_table *table;
	const t_input_args *input_args;
}				t_philosopher_args;

pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;

void init_philosopher(t_philosopher *philosopher, unsigned int number_philo, \
						unsigned int left_fork, unsigned int right_fork) {
    philosopher->number_philo = number_philo;
    philosopher->left_fork = left_fork;
    philosopher->right_fork = right_fork;
}
 
void init_table(t_table *table) {
    size_t i;
    for (i = 0; i < PHT_SIZE; i++)
        pthread_mutex_init(&table->forks[i], NULL);
}


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

void* eat(void *args) 
{
    t_philosopher_args *arg = (t_philosopher_args*)args;
    const t_philosopher *philosopher = arg->philosopher;
    const t_table *table = arg->table;
	const t_input_args *input_args = arg->input_args;
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
	else
	{
		while (times_eat < input_args->times_must_eat)
		{

			pthread_mutex_lock(&entry_point);
			pthread_mutex_lock(&table->forks[philosopher->left_fork]);
			printf("%u started dinner\n", philosopher->number_philo);
			usleep(input_args->time_eat);
			pthread_mutex_lock(&table->forks[philosopher->right_fork]);
			pthread_mutex_unlock(&entry_point);
			
			printf("%u is eating after %d ms sleep\n", philosopher->number_philo, r);
			
			pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
			pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
			
					
			printf("%u finished dinner\n", philosopher->number_philo);
			
			times_eat++;
		}
	}
}

void init_input_args(t_input_args *input_args, char **argv)
{
	if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
		exit(1);
	input_args->num_philo = atoi(argv[1]);
	input_args->time_die = atoi(argv[2]); 
	input_args->time_eat = atoi(argv[3]);
	input_args->time_sleep = atoi(argv[4]);
	input_args->times_must_eat = argv[1] ? atoi(argv[5]) : 0;
}

int main(int agrc, char **argv) 
{
	t_input_args input_args;

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