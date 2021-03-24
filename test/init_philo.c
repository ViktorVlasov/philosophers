/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:32:33 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/24 13:19:39 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static int	get_len(int n)
{
	int len;

	len = n == 0 ? 1 : 0;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	dig_to_char(int n)
{
	if (n < 0)
		return (-1 * n + '0');
	return (n + '0');
}

char		*ft_itoa(int n)
{
	char	*str;
	int		size;
	int		mark;

	mark = n < 0 ? 1 : 0;
	size = mark == 1 ? get_len(n) + 1 : get_len(n);
	str = (char *)malloc((size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[size] = '\0';
	while (size > mark)
	{
		size--;
		str[size] = dig_to_char(n % 10);
		n /= 10;
	}
	if (mark == 1)
	{
		size--;
		str[size] = '-';
	}
	return (str);
}

int					init_input_args(t_input_args *input_args, \
									char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		return (print_error(NUMBER_ARGS, NULL));
	input_args->amount_philo = ft_atoi(argv[1]);
	input_args->time_die = ft_atoi(argv[2]);
	input_args->time_eat = ft_atoi(argv[3]);
	input_args->time_sleep = ft_atoi(argv[4]);
	input_args->times_must_eat = argv[5] ? ft_atoi(argv[5]) : 0;
	if (input_args->amount_philo < 2 || input_args->amount_philo > 200 ||
		input_args->time_die < 60 || input_args->time_eat < 60 ||
		input_args->time_sleep < 60 || input_args->times_must_eat < 0)
		return (print_error(INCORRECT_ARGS, NULL));
	input_args->time_start = get_time();
	input_args->is_dead = 0;
	return (SUCCES);
}

static void			*ret_error(int err_f, t_philosopher_args *philo_args)
{
	if (err_f == 1)
		print_error(MALLOC_ERR, NULL);
	if (err_f == 2)
		print_error(MALLOC_ERR, philo_args);
	return (NULL);
}

// static void			init_philo(t_philosopher_args *philo_args,
// 								pthread_mutex_t *forks, int num_ph, int i)
// {
// 	philo_args[i].number_philo = i + 1;
// 	philo_args[i].left_fork = &forks[i];
// 	if (philo_args[i].number_philo == num_ph)
// 		philo_args[i].right_fork = &forks[0];
// 	else
// 		philo_args[i].right_fork = &forks[i + 1];
// 	philo_args[i].forks = forks;
// }

size_t	ft_strlen(const char *s)
{
	size_t i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	size_t	i;
	char	*res;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	res = (char*)malloc((size + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (*s1)
		res[i++] = *s1++;
	while (*s2)
		res[i++] = *s2++;
	res[i] = '\0';
	return (res);
}

void init_checker(t_philosopher_args *philo_args, int i)
{
	char *num_i;

	num_i = ft_itoa(i);
	philo_args[i].checker_name = ft_strjoin("checker", num_i);
	sem_unlink(philo_args[i].checker_name);
	philo_args[i].checker_mutex = sem_open(philo_args[i].checker_name,
						O_CREAT | O_EXCL, 0644, 1);
	free(num_i);
}

t_philosopher_args	*init_philosopher_args(t_input_args *input_args)
{
	sem_t				*forks;
	t_philosopher_args	*philo_args;
	int					i;
	// sem_t				*checker;

	if (!(philo_args = malloc(sizeof(t_philosopher_args) *
		input_args->amount_philo)))
		return (ret_error(1, NULL));
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT | O_EXCL, 0644, input_args->amount_philo);
	// sem_unlink("checker");
	// checker = sem_open("checker", O_CREAT | O_EXCL, 0644, 1);
	i = -1;
	while (++i < input_args->amount_philo)
	{
		philo_args[i].number_philo = i + 1;
		philo_args[i].forks = forks;
		philo_args[i].input_args = input_args;
		philo_args[i].last_meal = 0;
		//philo_args[i].checker_mutex = checker;
		init_checker(philo_args, i);
		//philo_args[i].count_meal = 0;
	}
	return (philo_args);
}
