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
	int	sign;
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

/*
** We can't pass in arguments when calling the handler function,
** hence we need a global/static variable to store the previous memory.
** Create a global struct if it only allows one global variable but you need more than one.
*/

static void	ft_receiver(int signum, siginfo_t *info, void *context)
{
	static int	bits = 0;
	static int	ascii = 0;

	(void)context;
	/*
	** We received bits starting from the left to theright by adding 1/0 and keep shifting.
	** SIGUSR1 represents the bit 1 while SIGUSR2 represents the bit 0.
	*/
	if (signum == SIGUSR1)
		ascii = (ascii << 1) + 0b000000001;
	else if (signum == SIGUSR2)
		ascii = ascii << 1;
	/* 
	** Print out one character at once (but it's inefficient)
	**
	** We can increase the efficiency of the program by reduce the number of calling write() function. 
	** For example, create a static buffer (using array char[ ]) to store the characters received until a certain point 
	** or until it detects the end only print out the words at once.
	*/
	if (++bits == 8)
	{
		/*
		** At the end of tranmission, send SIGUSR1 to client to inform the client,
		** representing everything works smoothly.
		*/
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
	sigset_t		ss;

	ft_putstr("\e[1;97;44m-------- Welcome to Minitalk --------\033[0m\n");
	ft_putstr("\033[35;7mThe server's current PID is:\033[0m ");
	// Get the process ID of the current process
	ft_putnbr((int)(getpid()));
	ft_putstr("\n");
	// reset the signal set ss by excluding everything first, then add in SIGUSR1 and SIGUSR2
	sigemptyset(&ss);
	sigaddset(&ss, SIGUSR1);
	sigaddset(&ss, SIGUSR2);
	// initialize the struct sigaction that stored the action associated with signals
	sa.sa_sigaction = &ft_receiver;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_mask = ss;
	// handle the signals with functions when the specified signals are caught
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	/*
	** pause() causes the program to sleep after finishing handling all the signals received.
	** The thread will constantly wait and be prepared to receive signals from the client,
	** until a signal is delivered (eg. from terminal) to terminates the process or causes 
	** the invocation of a signal-catching function.
	*/
	while (42)
		pause();
	return (0);
}
