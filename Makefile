NAME		=	minishell
NAME_D		=	$(NAME)_debug

CC			=	clang
FLAGS		=	-Wall -Werror -Wextra
FLAGS_D		=	-g

LIB_NAME_S		=	ft
LIB_NAME_S_D	=	ft_debug
LIB_NAME		=	lib$(LIB_NAME_S).a
LIB_NAME_D		=	lib$(LIB_NAME_S_D).a
LIB_PATH		=	lib/libft
LIB				=	$(LIB_PATH)/$(LIB_NAME)
LIB_D			=	$(LIB_PATH)/$(LIB_NAME_D)

LIB_V_NAME_S	=	vector
LIB_V_NAME_S_D	=	vector_debug
LIB_V_NAME		=	lib$(LIB_V_NAME_S).a
LIB_V_NAME_D	=	lib$(LIB_V_NAME_S_D).a
LIB_V_PATH		=	lib/libvector
LIB_V			=	$(LIB_V_PATH)/$(LIB_V_NAME)
LIB_V_D			=	$(LIB_V_PATH)/$(LIB_V_NAME_D)

LINK_LIB	=	-lreadline -l$(LIB_NAME_S) -L$(LIB_PATH) -l$(LIB_V_NAME_S) -L$(LIB_V_PATH)
#LINK_LIB	=	-lreadline -L Users/tim/.brew/opt/readline/lib -I /Users/tim/.brew/opt/readline/include -l$(LIB_NAME_S) -L$(LIB_PATH) -l$(LIB_V_NAME_S) -L$(LIB_V_PATH)
LINK_LIB_D	=	-lreadline -l$(LIB_NAME_S_D) -L$(LIB_PATH) -l$(LIB_V_NAME_S_D) -L$(LIB_V_PATH)
#LINK_LIB_D	=	-lreadline -L /Users/tim/.brew/opt/readline/lib -I /Users/tim/.brew/opt/readline/include -l$(LIB_NAME_S_D) -L$(LIB_PATH) -l$(LIB_V_NAME_S_D) -L$(LIB_V_PATH)

SRCS		=	$(addprefix srcs/,\
				signal.c prompt.c utilities.c utilities_vector.c utilities_readline.c commands.c commands_2.c \
				exit.c deinit.c utilities_variable_env.c  init.c error.c minishell.c \
				lexer.c ft_isspace.c in_out_files.c ft_errors_managment.c here_doc.c get_next_line.c )

HEADERS		=	includes/minishell.h

OBJS		=	${SRCS:%.c=%.o}
OBJS_D		=	${SRCS:%.c=%_debug.o}

INCLUDES	=	-I includes
RM			=	rm -rf

OS				=	${shell uname -s}

NOCOLOR			=	\033[0m
COLOR_LGREEN	=	\033[92m
COLOR_LYELLOW	=	\033[93m
COLOR_LCYAN		=	\033[96m
NEWLINE			=	\n

%.o			:	%.c $(HEADERS)
				@$(CC) ${INCLUDES} $(FLAGS) -c $< -o $@
				@printf "$(COLOR_LCYAN)build object$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$@$(NOCOLOR)$(NEWLINE)"

%_debug.o	:	%.c $(HEADERS)
				@$(CC) ${INCLUDES} $(FLAGS_D) $(FLAGS) -DDEBUG_MODE -c $< -o $@
				@printf "$(COLOR_LCYAN)build object$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$@$(NOCOLOR)$(NEWLINE)"

.phony		:	all debug clean fclean re norm

all			:	$(NAME)

debug		:	${NAME_D}

$(NAME)		:	$(LIB) $(LIB_V) $(OBJS)
				@$(CC) $(OBJS) $(LINK_LIB) -o $(NAME)
				@printf "$(COLOR_LCYAN)link$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$(NAME)$(NOCOLOR) for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

$(NAME_D)	:	$(LIB_D) $(LIB_V_D) $(OBJS_D)
				@$(CC) $(OBJS_D) $(LINK_LIB_D) -o $(NAME_D)
				@printf "$(COLOR_LCYAN)link debug$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$(NAME_D)$(NOCOLOR) for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

$(LIB)		:	
				@$(MAKE) -s -C $(LIB_PATH)

$(LIB_D)		:	
				@$(MAKE) debug -s -C $(LIB_PATH) 

$(LIB_V)		:
				@$(MAKE) -s -C $(LIB_V_PATH)

$(LIB_V_D)		:
				@$(MAKE) debug -s -C $(LIB_V_PATH)

clean		:	
				@$(RM) $(OBJS) $(OBJS_D)
				@$(MAKE) clean -s -C $(LIB_PATH)
				@$(MAKE) clean -s -C $(LIB_V_PATH)
				@printf "$(COLOR_LCYAN)$@$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$(NAME)$(NOCOLOR) for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

fclean		:	clean
				@$(RM) $(NAME) $(NAME_D)
				@$(MAKE) fclean -s -C $(LIB_PATH)
				@$(MAKE) fclean -s -C $(LIB_V_PATH)
				@printf "$(COLOR_LCYAN)$@$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$(NAME)$(NOCOLOR) for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

re			:	fclean all
				@printf "$(COLOR_LCYAN)$@$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready $(COLOR_LYELLOW)$(NAME)$(NOCOLOR) for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

norm		:
				@norminette $(SRCS) $(HEADERS)
				@$(MAKE) norm -s -C $(LIB_PATH)
				@printf "$(COLOR_LCYAN)norminette$(NOCOLOR) [$(COLOR_LGREEN)info$(NOCOLOR)]: "
				@printf "ready for $(COLOR_LYELLOW)$(OS)$(NOCOLOR)$(NEWLINE)"

pd			:
				@$(MAKE) debug -s -C $(LIB_PATH)
				@$(MAKE) debug -s -C $(LIB_V_PATH)
				clang -Wall -Wextra -Werror srcs/lexer.c srcs/ft_token.c srcs/ft_token_utils.c srcs/ft_isspace.c -L./lib/libft -lft -L./lib/libvector -lvector -lreadline -g -Iincludes

print-%  	: ; @echo $* = $($*)