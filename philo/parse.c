/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aolde-mo <aolde-mo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 12:58:48 by aolde-mo          #+#    #+#             */
/*   Updated: 2023/05/26 19:06:54 by aolde-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	parse_correct_arguments(t_data *data, int ms_or_no, int i)
{
	if (i == 1)
		data->no_of_philosophers = ms_or_no;
	else if (i == 2)
		data->time_to_die = ms_or_no;
	else if (i == 3)
		data->time_to_eat = ms_or_no;
	else if (i == 4)
	{
		data->time_to_sleep = ms_or_no;
		data->times_philo_must_eat = 0;
	}
	else
		data->times_philo_must_eat = ms_or_no;
}

int	parse_data(t_data *data, char **argv)
{
	int	i;
	int	ms;

	i = 1;
	while (argv[i])
	{
		ms = ft_atoi_data(argv[i]);
		if (ms == -1)
			return (1);
		parse_correct_arguments(data, ms, i);
		i++;
	}
	data->status = malloc(sizeof(t_status));
	if (!data->status)
		return (1);
	data->status->has_eaten_enough = 0;
	data->status->all_ate = false;
	data->status->someone_died = false;
	gettimeofday(&data->tv, NULL);
	data->start_time = data->tv.tv_sec * 1000LL + data->tv.tv_usec / 1000LL;
	return (0);
}

int	parse_philo(t_data *data)
{
	int		i;
	int		no;

	i = 0;
	no = data->no_of_philosophers;
	data->th = malloc(no * sizeof(pthread_t));
	if (!data->th)
		return (1);
	data->philo = malloc(no * sizeof(t_philo));
	if (!data->philo)
		return (2);
	while (i < no)
	{
		data->philo[i].philo_no = i + 1;
		data->philo[i].last_time_eaten = 0;
		data->philo[i].times_eaten = 0;
		data->philo[i].is_eating = false;
		data->philo[i].data = data;
		data->philo[i].fork = malloc(sizeof(pthread_mutex_t));
		if (!data->philo[i].fork \
			|| pthread_mutex_init(data->philo[i].fork, NULL))
			return (3 + i);
		i++;
	}
	return (0);
}

void	parse_mutex(t_data *data)
{
	int		i;
	int		no;

	i = 0;
	no = data->no_of_philosophers;
	while (i < no)
	{
		data->philo[i].l_fork = data->philo[i].fork;
		data->philo[i].r_fork = data->philo[(i + 1) % no].fork;
		i++;
	}
}

int	parse_all(t_data *data, char **argv)
{
	int	err;

	if (parse_data(data, argv))
		return (1);
	err = parse_philo(data);
	if (err)
	{
		free_specific(data, err);
		return (1);
	}
	parse_mutex(data);
	return (0);
}
