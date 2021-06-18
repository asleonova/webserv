# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wnormcor <wnormcor@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/17 10:37:19 by wnormcor          #+#    #+#              #
#    Updated: 2021/06/17 00:13:34 by wnormcor         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	webserv

SRCS =	main.cpp \
		config.cpp \
		request.cpp \
		response.cpp \
		handlerequest.cpp \
		utils.cpp 

OBJS = $(addprefix obj/, $(SRCS:%.cpp=%.o))

all: $(NAME)

$(NAME): $(OBJS)
	clang++ -Wall -Wextra -Werror -std=c++98 -g $(OBJS) -o $(NAME)

obj/%.o : src/%.cpp src/%.hpp src/includes.hpp
	@mkdir -p obj
	clang++ -Wall -Wextra -Werror -std=c++98 -g -c $< -o $@

clean:
	@if [ -d obj ]; then rm -r obj/; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	clang++ -std=c++98 src/cgihandler.cpp src/main_test_cgi.cpp src/request.cpp

.PHONY: all clean fclean re
