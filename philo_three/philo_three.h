/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 01:29:12 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/27 02:04:15 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <stdint.h>

# define MS 1000
# define SUCCES 0
# define FAIL 1
# define DIE 11

# define NUMBER_ARGS 1
# define INCORRECT_ARGS 2
# define MALLOC_ERR 3

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "is died"

typedef struct		s_input_args {
	int				amount_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				times_must_eat;
	int				time_start;
	int				is_dead;
}					t_input_args;

typedef struct		s_philosopher_args {
	unsigned int	number_philo;
	t_input_args	*input_args;
	sem_t			*forks;
	sem_t			*checker_sem;
	pthread_t		checker_thread;
	unsigned int	last_meal;
	pid_t			pid_id;
	sem_t			*sem_print;
	sem_t			*sem_block;
}					t_philosopher_args;

int					init_input_args(t_input_args *input_args,\
									char **argv,\
									int argc);
t_philosopher_args	*init_philosopher_args(t_input_args *input_args, int i);
void				free_all(t_philosopher_args *philo_args);
int					ft_atoi(const char *str);
unsigned int		get_time();
int					print_error(int code_error, t_philosopher_args *p_args);
void				kill_process(t_philosopher_args *philo_args);
int					wait_process(t_philosopher_args *philo_args);
int					start_p(t_philosopher_args *philo_args,\
							t_input_args input_args);
int					eat(t_philosopher_args *p_args, int *count_meal);
void				*philosophize(void *args);

#endif
