#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PHT_SIZE 5

typedef struct	s_input_args {
	unsigned int amount_philo;
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

typedef struct	s_philosopher_args {
    t_philosopher philosopher;
    pthread_mutex_t *forks;
	const t_input_args *input_args;
}				t_philosopher_args;