# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmehran <mmehran@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/03 17:46:50 by mmehran           #+#    #+#              #
#    Updated: 2021/07/03 17:48:16 by mmehran          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
LIBS = -lpthread

OBJS =
OBJS_BONUS =

ifdef WITH_BONUS
OBJS = $(OBJS_BONUS)
endif

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	$(RM) $(NAME)

re: clean all

bonus:
	$(MAKE) WITH_BONUS=1 all

.PHONY: all clean fclean re bonus
