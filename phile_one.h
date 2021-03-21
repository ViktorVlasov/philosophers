/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phile_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:36:21 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/21 21:13:13 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>

# define MS 1000
# define SUCCES 0
# define FAIL 1

# define NUMBER_ARGS 1
# define INCORRECT_ARGS 2
# define MALLOC_ERR 3

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "is died"


typedef struct	s_input_args {
	unsigned int	amount_philo;
	unsigned int	time_die; 
	unsigned int	time_eat;
	unsigned int	time_sleep;
	unsigned int	times_must_eat;
	unsigned int	time_start;
	int				is_dead;
}				t_input_args;

typedef struct	s_philosopher {
	unsigned int	number_philo;
	unsigned int	left_fork;
	unsigned int	right_fork;
}				t_philosopher;

typedef struct	s_philosopher_args {
    t_philosopher	philosopher;
	t_input_args	*input_args;
    pthread_mutex_t	*forks;
	pthread_mutex_t	checker_mutex;
	pthread_t		checker_thread;
	unsigned int	last_meal;
}				t_philosopher_args;


void 				init_input_args(t_input_args *input_args, \
									char **argv, \
									int argc);
void 				init_philosopher(t_philosopher *philosopher,\
										unsigned int number_philo, \
									unsigned int left_fork, \
									unsigned int right_fork);
t_philosopher_args	*init_philosopher_args(t_input_args *input_args);
void				free_all(t_philosopher_args *philo_args);
int					ft_atoi(const char *str);
unsigned int		get_time();
int				print_error(int code_error, t_philosopher_args *p_args);

#endif
