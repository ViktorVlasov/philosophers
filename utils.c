/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:55:08 by efumiko           #+#    #+#             */
/*   Updated: 2021/03/20 19:55:16 by efumiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phile_one.h"

int	ft_atoi(const char *str)
{
	int	nb;
	int	mark;

	mark = 0;
	nb = 0;
	while (*str == ' ' || *str == '\n' || *str == '\r' ||
		*str == '\v' || *str == '\f' || *str == '\t')
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			mark = 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nb = 10 * nb + (*str++ - '0');
	}
	if (mark == 1)
	{
		return (-1 * nb);
	}
	return (nb);
}