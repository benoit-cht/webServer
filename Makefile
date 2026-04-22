# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/20 09:10:14 by bchallat          #+#    #+#              #
#    Updated: 2026/04/22 11:13:39 by bchallat         ###   ########.fr        #
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
			network/SocketHandler.cpp\
			network/ClientInfo.cpp \
			network/ClientManager.cpp \
			network/EventLoop.cpp \
			request/HttpHandler.cpp \
			request/req_lexing.cpp \
			request/req_descending_deriv.cpp \
			request/req_struct_http_request.cpp \
			request/readFile.cpp \
			request/httpResponse.cpp \
			request/servStaticFile.cpp \

SRCS = ${addprefix ${SRCDIR}/, ${SRC}}
OBJS = ${addprefix ${OBJDIR}/, $(SRC:.cpp=.o)}

object/%.o:sources/%.cpp | $(OBJDIR)
	@echo  "$(GREEN)[OK]       	🛠️ $(YELLOW)Compaling ... $(BLUE)$< $(RESET)"
	@$(CCFLAG) -c $< -o $@


#=======================================================================#
#                                                                       #
#=======================================================================#

all: $(NAME)

$(NAME): $(OBJS)
	@echo  "$(GREEN)[OK]       	✅ $(YELLOW)CREATED   EXE$(BLUE) -- webserv --\n $(RESET)" 
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

${OBJDIR}:
	@echo "${GREEN}[->]		${YELLOW}📂 Creating directory ${OBJ_DIR}$(BLUE)object/..$(RESET)\n"
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OBJDIR}/network 
	@mkdir -p ${OBJDIR}/cgi 
	@mkdir -p ${OBJDIR}/request
	@mkdir -p ${OBJDIR}/routing

