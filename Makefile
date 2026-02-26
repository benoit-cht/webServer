# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/20 09:10:14 by bchallat          #+#    #+#              #
#    Updated: 2026/02/26 15:30:56 by bchallat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#=======================================================================#
#                                                                       #
#=======================================================================#

GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
RED 	= \033[0;91m
RESET	= \033[0m

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CC	= clang++
	FLAG	= -g3 -Wall -Wextra -Werror -std=c++98 -fsanitize=address
else
	CC	= c++
	FLAG	= -Wall -Wextra -Werror -std=c++98
endif

CCFLAG	= $(CC) $(FLAG) 

SRCDIR = sources
OBJDIR = object

RMF = rm -rf

NAME = webserv

#=======================================================================#
#                                                                       #
#=======================================================================#

SRC	=	webserver.cpp \
			socket/Socket.cpp \
			socket/Clients.cpp

SRCS = ${addprefix ${SRCDIR}/, ${SRC}}
OBJS = ${addprefix ${OBJDIR}/, $(SRC:.cpp=.o)}

#=======================================================================#
#                                                                       #
#=======================================================================#

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)[OK]       	✅ $(YELLOW)Compiling END$(BLUE) - all file obj -$(RESET)"
	@echo "$(GREEN)[OK]       	✅ $(YELLOW)CREATED   EXE$(BLUE) -- webserv.exe --\n $(RESET)" 
	@$(CCFLAG) -g3 $(OBJS) -o $(NAME)

clean:
	@echo "$(RED)[DELETE]	🧹$(BLUE) Delete file obj $(RESET)\n" 
	@$(RMF) $(OBJDIR) $(OBJS)

fclean:
	@echo "$(RED)[DELETE]	🧹$(BLUE) Delete all file obj and .exe $(RESET)\n"
	@$(RMF) $(OBJDIR) $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re

#===============================================================#
#                                                               #
#===============================================================#

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "$(BLUE)[..]       	🛠️ $(YELLOW)Compiling ... $(BLUE)$<$(RESET)"
	@$(CCFLAG) -c $< -o $@ >/dev/null 2>&1

${OBJDIR}:
	@echo "${GREEN}[->]		${YELLOW}📂 Creating directory ${OBJ_DIR}$(BLUE)object/..$(RESET)"
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OBJDIR}/socket 

