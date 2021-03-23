/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/23 22:58:40 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

sem_t *g_print;

void	print_mesg(t_philosopher_args *p_args, char *mesg)
{
	unsigned int t_start;

	t_start = p_args->input_args->time_start;
	sem_wait(g_print);
	if (p_args->input_args->is_dead)
	{
		sem_post(g_print);
		return ;
	}
	printf("%u %d %s\n", get_time() - t_start, \
		p_args->number_philo, mesg);
	sem_post(g_print);
}

int		eat(t_philosopher_args *p_args, int *count_meal)
{
	if (p_args->input_args->is_dead)
		return (FAIL);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->checker);
	print_mesg(p_args, MSG_EAT);
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	sem_post(p_args->checker);
	(*count_meal)++;
	usleep(p_args->input_args->time_eat * MS);
	sem_post(p_args->forks);
	sem_post(p_args->forks);
	return (SUCCES);
}

void	*philosophize(void *args)
{
	t_philosopher_args	*p_args;
	int					count_meal;

	p_args = (t_philosopher_args*)args;
	count_meal = 0;
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_create(&p_args->checker_thread, NULL, check_death, p_args);
	if (p_args->number_philo % 2 == 0)
		usleep(MS * p_args->input_args->time_eat);
	while (!p_args->input_args->is_dead)
	{
		eat(p_args, &count_meal);
		if ((p_args->input_args->times_must_eat != 0 &&
			count_meal == p_args->input_args->times_must_eat) ||
			p_args->input_args->is_dead)
			exit(FAIL);
		print_mesg(p_args, MSG_SLEEP);
		if (p_args->input_args->is_dead)
			exit(FAIL);
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
	}
	if (p_args->input_args->is_dead)
		exit(FAIL);
	exit(SUCCES);
}

void			free_all(t_philosopher_args *philo_args, pthread_t *phil)
{
	sem_close(g_print);
	sem_unlink("g_print");
	if (phil)
		free(phil);
	if (philo_args)
	{
		sem_close(philo_args->fix);
		sem_close(philo_args->checker);
		sem_close(philo_args->forks);
		sem_unlink("fix");
		sem_unlink("checker");
		sem_unlink("forks");
		free(philo_args);
	}
}

void	*check_death(void *args)
{
	t_philosopher_args *p_args;

	p_args = (t_philosopher_args*)args;
	pthread_detach(p_args->checker_thread);
	while (!p_args->input_args->is_dead)
	{
		sem_wait(p_args->checker);
		if (!p_args->input_args->is_dead && get_time() > p_args->last_meal)
		{
			print_mesg(p_args, MSG_DIED);
			sem_wait(g_print);
			p_args->input_args->is_dead = 1;
			exit(1);
		}
		sem_post(p_args->checker);
		usleep(MS);
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	t_input_args		input_args;
	t_philosopher_args	*philo_args;
	pthread_t			*phil;
	int					i;

	sem_unlink("g_print");
	g_print = sem_open("g_print", O_CREAT | O_EXCL, 0644, 1);
	init_input_args(&input_args, argv, argc);
	if (!(philo_args = init_philosopher_args(&input_args)))
		return (1);
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		return (print_error(MALLOC_ERR, philo_args));
	i = -1;
	while (++i < input_args.amount_philo)
	{
		if (!(philo_args[i].pid_id = fork()))
			philosophize(&philo_args[i]);
		else if (philo_args[i].pid_id < 0)
		{
			kill_process(philo_args);
			free_all(philo_args, phil);
			return (FAIL);
		}
	}
	if (wait_process(philo_args) == FAIL)
	{
		free_all(philo_args, phil);
		return (FAIL);
	}
	free_all(philo_args, phil);
	exit(0);
}