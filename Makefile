# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/20 09:10:14 by bchallat          #+#    #+#              #
#    Updated: 2026/05/26 20:24:31 by tjacquel         ###   ########.fr        #
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

CCFLAG	= $(CC) $(FLAG) -I./include -MMD -MP

SRCDIR = sources
OBJDIR = object

RMF = rm -rf

NAME = webserv

#=======================================================================#
#                                                                       #
#=======================================================================#

SRC	=	webserver.cpp \
			config/ServerConfig.cpp \
			config/LocationConfig.cpp \
			config/ConfigParsing.cpp \
			config/ConfigParsingUtils.cpp \
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
			cgi/CgiHandler.cpp \


SRCS = ${addprefix ${SRCDIR}/, ${SRC}}
OBJS = ${addprefix ${OBJDIR}/, $(SRC:.cpp=.o)}

object/%.o:sources/%.cpp | $(OBJDIR)
	@echo -e  "$(GREEN)[OK]       	🛠️ $(YELLOW)Compiling ... $(BLUE)$< $(RESET)"
	@$(CCFLAG) -c $< -o $@


#=======================================================================#
#                                                                       #
#=======================================================================#

all: $(NAME)

$(NAME): $(OBJS)
	@echo -e  "$(GREEN)[OK]       	✅ $(YELLOW)CREATED   EXE$(BLUE) -- webserv --\n $(RESET)"
	@$(CCFLAG) -g3 $(OBJS) -o $(NAME)

clean:
	@echo -e "$(RED)[DELETE]	🧹$(BLUE) Delete file obj $(RESET)\n"
	@$(RMF) $(OBJDIR) $(OBJS)

fclean:
	@echo -e "$(RED)[DELETE]	🧹$(BLUE) Delete all file obj and .exe $(RESET)\n"
	@$(RMF) $(OBJDIR) $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re

#===============================================================#
#                                                               #
#===============================================================#

${OBJDIR}:
	@echo -e "${GREEN}[->]		${YELLOW}📂 Creating directory ${OBJ_DIR}$(BLUE)object/..$(RESET)\n"
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OBJDIR}/network
	@mkdir -p ${OBJDIR}/cgi
	@mkdir -p ${OBJDIR}/config
	@mkdir -p ${OBJDIR}/request
	@mkdir -p ${OBJDIR}/routing

