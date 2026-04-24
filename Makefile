# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/20 09:10:14 by bchallat          #+#    #+#              #
#    Updated: 2026/04/24 16:28:46 by lcluzan          ###   ########.fr        #
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

CCFLAG	= $(CC) $(FLAG) -I./include

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
			cgi/cgiHandler.cpp \

SRCS = ${addprefix ${SRCDIR}/, ${SRC}}
OBJS = ${addprefix ${OBJDIR}/, $(SRC:.cpp=.o)}

object/%.o:sources/%.cpp | $(OBJDIR)
	@echo  "$(GREEN)[OK]       	🛠️ $(YELLOW)Compiling ... $(BLUE)$< $(RESET)"
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
	@$(RMF) $(OBJDIR) $(OBJS) $(NAME) $(TEST_NAME)

re: fclean all

#===============================================================#
#                                                               #
#===============================================================#

TEST_DIR = stack/tests
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJDIR)/%.o, $(TEST_SRCS))

TEST_NAME = test_webserv

$(TEST_NAME): $(TEST_OBJS) $(OBJS)
	@echo "$(GREEN)[OK]       	🧪 $(YELLOW)Compiling tests...$(RESET)"
	@$(CCFLAG) $(filter-out $(OBJDIR)/webserver.o, $(OBJS)) $(TEST_OBJS) -o $(TEST_NAME)

$(OBJDIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJDIR)
	@echo "$(GREEN)[OK]       	🛠️ $(YELLOW)Compiling test... $(BLUE)$<$(RESET)"
	@$(CCFLAG) -Iinclude -I$(TEST_DIR)/include -c $< -o $@

test: $(TEST_NAME)
	@echo "$(GREEN)[OK]       	🧪 $(YELLOW)Running tests...$(RESET)"
	@./$(TEST_NAME)

.PHONY: test all clean fclean re

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

