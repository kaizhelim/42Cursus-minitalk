/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalim <kalim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 19:57:17 by kalim             #+#    #+#             */
/*   Updated: 2021/07/02 19:57:17 by kalim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	ft_putnbr(int n)
{
	int		sign;
	char	c;

	sign = 1;
	if (n < 0)
	{
		write(1, "-", 1);
		sign = -1;
	}
	if (n / 10)
		ft_putnbr(sign * (n / 10));
	c = '0' + sign * (n % 10);
	write(1, &c, 1);
}

static void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
}

static void	ft_receiver(int signum, siginfo_t *info, void *context)
{
	static int	bits = 0;
	static int	ascii = 0;

	(void)context;
	if (signum == SIGUSR1)
		ascii = (ascii << 1) + 0b000000001;
	else if (signum == SIGUSR2)
		ascii = ascii << 1;
	if (++bits == 8)
	{
		if (ascii == EOT)
		{
			kill(info->si_pid, SIGUSR1);
			write(1, "\n", 1);
		}
		else
			write(1, &ascii, 1);
		bits = 0;
		ascii = 0;
	}
}

int	main(void)
{
	struct sigaction	sa;
	sigset_t			ss;

	ft_putstr("\e[1;97;44m-------- Welcome to Minitalk --------\033[0m\n");
	ft_putstr("\033[35;7mThe server's current PID is:\033[0m ");
	ft_putnbr((int)(getpid()));
	ft_putstr("\n");
	sigemptyset(&ss);
	sigaddset(&ss, SIGUSR1);
	sigaddset(&ss, SIGUSR2);
	sa.sa_sigaction = &ft_receiver;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_mask = ss;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (42)
		pause();
	return (0);
}
