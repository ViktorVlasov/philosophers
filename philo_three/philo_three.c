/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/25 02:28:10 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

sem_t *g_print;

void	print_mesg(t_philosopher_args *p_args, char *mesg)
{
	unsigned int t_start;

	t_start = p_args->input_args->time_start;
	sem_wait(g_print);
	printf("%d %d %s\n", (int)(get_time() - t_start), \
		p_args->number_philo, mesg);
	sem_post(g_print);
}

void		eat(t_philosopher_args *p_args)
{
	if (p_args->input_args->is_dead || (p_args->input_args->times_must_eat &&\
		p_args->count_meal == p_args->input_args->times_must_eat))
		return ;
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->forks);
	print_mesg(p_args, MSG_FORK);
	sem_wait(p_args->checker);
	print_mesg(p_args, MSG_EAT);
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	sem_post(p_args->checker);
	p_args->count_meal++;
	usleep(p_args->input_args->time_eat * MS);
	sem_post(p_args->forks);
	sem_post(p_args->forks);
	return ;
}

void	free_all(t_philosopher_args *philo_args)
{
	sem_close(g_print);
	sem_unlink("g_print");
	if (philo_args)
	{
		sem_close(philo_args->checker);
		sem_close(philo_args->forks);
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
			exit(DIED);
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
	int					i;

	sem_unlink("g_print");
	g_print = sem_open("g_print", O_CREAT | O_EXCL, 0644, 1);
	if (init_input_args(&input_args, argv, argc) ||
		!(philo_args = init_philosopher_args(&input_args)))
		return (FAIL);
	if (start_p(philo_args, input_args) == FAIL)
		return (FAIL);
	free_all(philo_args);
	exit(SUCCES);
}
