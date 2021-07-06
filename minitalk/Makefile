# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kalim <kalim@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/29 11:46:31 by kalim             #+#    #+#              #
#    Updated: 2021/06/29 11:46:31 by kalim            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

/*
** ---------------DEFECT---------------
** (Standardised rules mentioned in C-Piscine C09 videos)
** "clean" should remove only the .o objects but not executable files
** "fclean" only remove both objects and executables
**
** In this project, my object file is also my executable file 
** so clean & fclean didn't work properly as expected.
*/

CLIENT 		= client
SERVER 		= server

CC		= gcc
CFLAGS		= -Wall -Wextra -Werror

all		: $(CLIENT) $(SERVER)
bonus		: $(CLIENT) $(SERVER)
fclean		: clean
re		: clean all

$(CLIENT):
	$(CC) $(CFLAGS) client.c -o $(CLIENT)

$(SERVER):
	$(CC) $(CFLAGS) server.c -o $(SERVER)

clean:
	rm -f $(CLIENT) $(SERVER)

.PHONY	: all clean fclean re bonus
