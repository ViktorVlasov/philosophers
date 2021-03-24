/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 12:29:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/24 10:28:33 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

sem_t *g_print;

void	print_mesg(t_philosopher_args *p_args, char *mesg)
{
	unsigned int t_start;

	t_start = p_args->input_args->time_start;
	sem_wait(g_print);
	if (p_args->input_args->is_dead || p_args->input_args->nb_ph_ate == p_args->input_args->amount_philo)
	{
		sem_post(g_print);
		return ;
	}
	if (mesg)
		printf("%u %d %s\n", get_time() - t_start, \
			p_args->number_philo, mesg);
	else
		printf("%u %d count meal: %d\n", get_time() - t_start, \
			p_args->number_philo, p_args->count_meal);
	sem_post(g_print);
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
			p_args->input_args->is_dead = 1;
			sem_post(p_args->checker);
			return (NULL);
		}
		sem_post(p_args->checker);
		usleep(MS);
	}
	return (NULL);
}

int		eat(t_philosopher_args *p_args)
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
	p_args->count_meal++;
	if (p_args->input_args->times_must_eat != 0 && p_args->count_meal == p_args->input_args->times_must_eat)
	{
			p_args->input_args->nb_ph_ate += 1;
	}	
	print_mesg(p_args, NULL);
	//(*count_meal)++;
	// if ((p_args->input_args->times_must_eat != 0 &&
	// 		count_meal == p_args->input_args->times_must_eat))
	// 	count_meal = -1;
	usleep(p_args->input_args->time_eat * MS);
	sem_post(p_args->forks);
	sem_post(p_args->forks);
	return (SUCCES);
}

void	*philosophize(void *args)
{
	t_philosopher_args	*p_args;
	int					max_eat;

	p_args = (t_philosopher_args*)args;
	p_args->last_meal = get_time() + p_args->input_args->time_die;
	pthread_create(&p_args->checker_thread, NULL, check_death, p_args);
	max_eat = p_args->input_args->times_must_eat;
	while (!p_args->input_args->is_dead)
	{
		eat(p_args);
		// if (max_eat != 0 && p_args->count_meal == max_eat)
		// {
		// 	p_args->input_args->nb_ph_ate += 1;
		// 	max_eat = 0;
		// }	
		if (p_args->input_args->nb_ph_ate == p_args->input_args->amount_philo)
		{
			p_args->input_args->is_dead = 1;
			break;
		}	
		if (p_args->input_args->is_dead)
			break ;
		print_mesg(p_args, MSG_SLEEP);
		if (p_args->input_args->is_dead)
			break ;
		usleep(p_args->input_args->time_sleep * MS);
		print_mesg(p_args, MSG_THINK);
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
	if (init_input_args(&input_args, argv, argc) ||
		!(philo_args = init_philosopher_args(&input_args)))
		return (1);
	if (!(phil = malloc(sizeof(pthread_t) * input_args.amount_philo)))
		return (print_error(MALLOC_ERR, philo_args));
	i = -1;
	while (++i < input_args.amount_philo)
	{
		pthread_create(&phil[i], NULL, philosophize, &philo_args[i]);
		usleep(MS);
	}
	i = -1;
	while (++i < input_args.amount_philo)
		pthread_join(phil[i], NULL);
	sem_close(g_print);
	sem_unlink("g_print");
	free(phil);
	free_all(philo_args);
}
