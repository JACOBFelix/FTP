CC	=	gcc

RM	=	rm -f

CFLAGS	+=	-Wall -Werror -Wextra -pedantic -W -I ./includes

DIRSERV	=	./srcs/server

DIRCLI	=	./srcs/client

DIRSHAR	=	./srcs/shared

DIRUSER	=	./srcs/user

SRCSSER	=	$(DIRSERV)/main.c \
		$(DIRSERV)/handle_user.c \
		$(DIRSERV)/handle_directory.c \
		$(DIRSERV)/handle_cmd_data.c \
		$(DIRSERV)/handler_command.c \
		$(DIRSERV)/handle_ls.c \
		$(DIRSERV)/handle_rm.c \
		$(DIRSERV)/handle_help_and_sys.c \
		$(DIRSHAR)/shared.c

SRCSCLI	=	$(DIRCLI)/main.c \
		$(DIRCLI)/arg_handler.c \
		$(DIRCLI)/cmd_cli_cd.c \
		$(DIRCLI)/cmd_cli_ls.c \
		$(DIRCLI)/cmd_cli_set.c \
		$(DIRCLI)/cmd_cli_pwd.c \
		$(DIRCLI)/cmd_cli_quit.c \
		$(DIRCLI)/cmd_cli_get.c \
		$(DIRCLI)/cmd_cli_ll.c \
		$(DIRCLI)/cmd_dir.c \
		$(DIRCLI)/cmd_cli_clear.c \
		$(DIRCLI)/cmd_cli_sys.c \
		$(DIRCLI)/cmd_cli_rm.c \
		$(DIRCLI)/cmd_cli_m.c \
		$(DIRSHAR)/shared.c

SRCSUSER=	$(DIRUSER)/main.c \
		$(DIRSHAR)/shared.c

OBJSSERV=	$(SRCSSER:.c=.o)

OBJSCLI	=	$(SRCSCLI:.c=.o)

OBJSUSER=	$(SRCSUSER:.c=.o)

NAME	=	serveur

NAME1	=	client

NAME2	=	add_user

%.o:		%.c
		@printf "[\033[0;32mCompiled\033[0m] % 35s\n" $< 
		@$(CC) -c -o $@ $< $(CFLAGS)

all:		$(NAME) $(NAME1) $(NAME2)

$(NAME):	$(OBJSSERV)
		@$(CC) $(OBJSSERV) -o $(NAME) $(CFLAGS)
		@printf "[\033[0;34mAssembled\033[0m] % 34s\n" $(NAME) | tr ' ' '.' 

$(NAME1):	$(OBJSCLI)
		@$(CC) $(OBJSCLI) -o $(NAME1) $(CFLAGS)
		@printf "[\033[0;34mAssembled\033[0m] % 34s\n" $(NAME1) | tr ' ' '.'

$(NAME2):	$(OBJSUSER)
		@$(CC) $(OBJSUSER) -o $(NAME2) $(CFLAGS)
		@printf "[\033[0;34mAssembled\033[0m] % 34s\n" $(NAME2) | tr ' ' '.'
clean:
		@$(RM) $(OBJSSERV)
		@printf "[\033[0;31mDeleted\033[0m] % 36s\n" $(OBJSSERV)
		@$(RM) $(OBJSCLI)
		@printf "[\033[0;31mDeleted\033[0m] % 36s\n" $(OBJSCLI)
		@$(RM) $(OBJSUSER)
		@printf "[\033[0;31mDeleted\033[0m] % 36s\n" $(OBJSUSER)

fclean:		clean
		@$(RM) $(NAME)
		@printf "[\033[0;35mRemoved\033[0m] % 36s\n" $(NAME) | tr ' ' '.'
		@$(RM) $(NAME1)
		@printf "[\033[0;35mRemoved\033[0m] % 36s\n" $(NAME1) | tr ' ' '.'
		@$(RM) $(NAME2)
		@printf "[\033[0;35mRemoved\033[0m] % 36s\n" $(NAME2) | tr ' ' '.'


re:		fclean all

.PHONY:		all clean fclean re
