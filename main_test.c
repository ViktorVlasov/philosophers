/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 16:10:34 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/19 21:10:04 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MS 1000

unsigned int	get_time(unsigned int start_usec, unsigned int start_sec)
{
	struct timeval	current_time;
	int				sec;

	gettimeofday(&current_time, NULL);
	sec = current_time.tv_sec - start_sec;
	return (((unsigned int)(sec * 1000000 + current_time.tv_usec) - start_usec)
	/ MS);
}

unsigned int	get_time_start(int time)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	if (time == 0)
		return (current_time.tv_usec);
	else
		return (current_time.tv_sec);
	return (0);
}

#define MICROSEC 0
#define SEC 1

int main(int argc, char **argv)
{
	unsigned int t_start_usec = get_time_start(MICROSEC);
	unsigned int t_start_sec = get_time_start(SEC);
    printf("MICROSEC: %d\n", t_start_usec);
    printf("SEC: %d\n", t_start_sec);
    
    usleep(100 * 1000);
    unsigned int test = get_time(t_start_usec, t_start_sec);
    printf("time: %d", test);
}