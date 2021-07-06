/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalim <kalim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 19:57:14 by kalim             #+#    #+#             */
/*   Updated: 2021/07/02 19:57:14 by kalim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

#define HINTS 0
#define FAILED 1
#define ACKNOWLEDGMENT 2

#define ATOI 0
#define PUTSTR 1

static pid_t	ft_utils(int usage, char *str)
{
	pid_t	id;
	int		i;

	id = 0;
	if (usage == ATOI)
	{
		while (*str)
		{
			if (!('0' <= *str && *str <= '9'))
				break ;
			id *= 10;
			id += (*str - '0');
			str++;
		}
	}
	else if (usage == PUTSTR)
	{
		i = -1;
		if (!str)
			return (id);
		while (str[++i])
			write(1, &str[i], 1);
	}
	return (id);
}

static int	print_message(int condition)
{
	if (condition == ACKNOWLEDGMENT)
	{
		ft_utils(PUTSTR, "\033[0;32mMessage successfully sent and printed");
		ft_utils(PUTSTR, "\033[0m\n");
		return (0);
	}
	else if (condition == HINTS)
	{
		ft_utils(PUTSTR, "\033[0;33mYour input should look like: ");
		ft_utils(PUTSTR, "./client [process_id] [message]\033[0m\n");
	}
	else if (condition == FAILED)
	{
		ft_utils(PUTSTR, "\033[0;31mYour message couldn’t be delivered");
		ft_utils(PUTSTR, "\033[0m\n");
		ft_utils(PUTSTR, "\033[0;31mPlease ensure that you enter a valid PID");
		ft_utils(PUTSTR, "\033[0m\n");
	}
	return (1);
}

static void	success(int signum)
{
	if (signum == SIGUSR1)
		print_message(ACKNOWLEDGMENT);
}

static short	send_signal(pid_t server_pid, char c)
{
	int	shift;

	shift = 8;
	while (--shift >= 0)
	{
		if (c & (0b000000001 << shift))
		{	
			if (kill(server_pid, SIGUSR1) == -1)
				return (0);
		}
		else
		{	
			if (kill(server_pid, SIGUSR2) == -1)
				return (0);
		}
		usleep(50);
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	pid_t	server_pid;
	int		i;

	if (argc != 3)
		return (print_message(HINTS));
	signal(SIGUSR1, success);
	server_pid = ft_utils(ATOI, argv[1]);
	i = 0;
	while (argv[2][i])
	{
		if (!send_signal(server_pid, argv[2][i]))
			return (print_message(FAILED));
		i++;
	}
	send_signal(server_pid, EOT);
	usleep(50);
	return (0);
}
