/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:42:20 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/23 20:38:16 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

// void	*check_death(void *args)
// {
// 	t_philosopher_args *p_args;

// 	p_args = (t_philosopher_args*)args;
// 	pthread_detach(p_args->checker_thread);
// 	while (!p_args->input_args->is_dead)
// 	{
// 		sem_wait(p_args->checker);
//         //sem_wait(p_args->fix);
// 		if (!p_args->input_args->is_dead && get_time() > p_args->last_meal)
// 		{
// 			print_mesg(p_args, MSG_DIED);
            
// 			p_args->input_args->is_dead = 1;
// 			//sem_post(p_args->checker);
// 			return (NULL);
// 		}
//         //sem_post(p_args->fix);
// 		sem_post(p_args->checker);
// 		usleep(MS);
// 	}
// 	return (NULL);
// }