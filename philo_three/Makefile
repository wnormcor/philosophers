# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/01 10:18:16 by wnormcor          #+#    #+#              #
#    Updated: 2021/02/22 12:50:59 by wnormcor         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	philo_three

SRCS =	philosophers.c \
		processes_philo.c \
		kapellmeister.c \
		utils.c \
		utils_time.c

OBJS = $(addprefix obj/, $(SRCS:%.c=%.o))

all: $(NAME)

$(NAME): $(OBJS)
	gcc -Wall -Wextra -Werror $(OBJS) -o $(NAME)

obj/%.o : src/%.c src/philosophers.h
	@mkdir -p obj
	gcc -Wall -Wextra -Werror -g -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d obj ]; then rmdir obj; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
