/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_twtwo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddraco <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/26 22:66:47 by ddraco           ###   ########.fr       */
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

void	*check_death(void *args)
{
	t_philosopher_args *p_args;

	p_args = (t_philosopher_args*)args;
	pthread_detach(p_args->checker_thread);
	while (!p_args->input_args->is_dead)
	{
		sem_wait(p_args->checker_sem);
		if (!p_args->input_args->is_dead && get_time() > p_args->last_meal)
		{
			print_mesg(p_args, MSG_DIED);
			sem_wait(g_print);
			p_args->input_args->is_dead = 1;
			//sem_post(p_args->checker_sem);
			exit (DIE);
		}
		sem_post(p_args->checker_sem);
		usleep(MS);
	}
	return (NULL);
}

sem_t *g_block;

int		eat(t_philosopher_args *p_args, int *count_meal)
{
	if (p_args->input_args->is_dead)
		exit (DIE);
	sem_wait(g_block);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->forks);	
	print_mesg(p_args, MSG_FORK);
	sem_post(g_block);
	sem_wait(p_args->checker_sem);
	print_mesg(p_args, MSG_EAT);
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	sem_post(p_args->checker_sem);
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
	while (!p_args->input_args->is_dead)
	{
		eat(p_args, &count_meal);
		if ((p_args->input_args->times_must_eat != 0 &&
			count_meal == p_args->input_args->times_must_eat))
			exit(SUCCES);
		if (p_args->input_args->is_dead)
			exit (DIE);
		print_mesg(p_args, MSG_SLEEP);
		if (p_args->input_args->is_dead)
			exit (DIE);
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
	}
}

void			kill_process(t_philosopher_args *philo_args)
{
	int i;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
		kill(philo_args[i].pid_id, SIGKILL);
}

int				wait_process(t_philosopher_args *philo_args)
{
	int i;
	int status;

	i = -1;
	while (++i < philo_args->input_args->amount_philo)
	{
		if (wait(&status) == -1)
			return (FAIL);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == DIE)
				kill_process(philo_args);		
		}
	}
	return (SUCCES);
}

int		start_p(t_philosopher_args *philo_args, t_input_args input_args)
{
	int i;
	
	i = -1;
	while (++i < input_args.amount_philo)
	{
		if (!(philo_args[i].pid_id = fork()))
			philosophize(&philo_args[i]);
		else if (philo_args[i].pid_id < 0)
		{
			kill_process(philo_args);
			free_all(philo_args);
			return (FAIL);
		}
		usleep(MS);
	}
	if (wait_process(philo_args) == FAIL)
	{
		free_all(philo_args);
		return (FAIL);
	}
	return (SUCCES);
}

int		main(int argc, char **argv)
{
	t_input_args		input_args;
	t_philosopher_args	*philo_args;
	int					i;

	sem_unlink("block");
	g_block = sem_open("block", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("g_print");
	g_print = sem_open("g_print", O_CREAT | O_EXCL, 0644, 1);
	if (init_input_args(&input_args, argv, argc) ||
		!(philo_args = init_philosopher_args(&input_args)))
		return (FAIL);
	if (start_p(philo_args, input_args) == FAIL)
		return (FAIL);
	sem_close(g_print);
	sem_unlink("g_print");
	free_all(philo_args);
	exit(SUCCES);
}
