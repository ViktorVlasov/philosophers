# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: efumiko <efumiko@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/23 21:49:08 by efumiko           #+#    #+#              #
#    Updated: 2021/03/26 23:58:55 by efumiko          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_three

SRC = philo_three.c init_philo.c utils.c

OBJ = $(SRC:.c=.o)

HEADER = philo_three.h

FLAGS = -Wall -Werror -Wextra -I./includes

all: $(NAME)

$(NAME):$(OBJ)
	cc -o $(NAME) $(FLAGS) $(OBJ) -lpthread

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re run