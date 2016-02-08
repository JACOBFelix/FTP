#ifndef CLIENT_H_
# define CLIENT_H_

#include <arpa/inet.h>
#include <netinet/in.h>

# define CLI_ARG_ERROR	"./client [Machine] [Port]\n"
# define INVALID_ADDR	"255.255.255.255"
# define INVALID_IP	"Server's IP adress is invalid\n"
# define NOT_A_IP	"It is not an adress IP\n"
# define SYNT_ERR_LINE_LONG	500
# define SYNT_ERR_PARAM_ARG	501
# define NOT_IMPL_SUPER_FLOUS	202
# define BAD_SEQUENCE_OF_CMD	503
# define CMD_NOT_IMPL_FOR_THAT_PARAM	504
# define LOG_ANO "Login as anonymous User\n"
# define ASK_PASS "What is your password?\n"
# define PROMPT_PASS "PASS : "
# define WHAT_NAME "What is your user name?\n"
# define WHAT_NAM_LEN strlen(WHAT_NAME)
# define PROMPT "(jacob_f:FTP)"

typedef struct	s_infos
{
  char		*path;
  char		*user_name;
}		t_infos;

int		cmd_cli_mget(int const,
			     const char *const);
int		cmd_cli_mset(int const,
			     const char *const);
int		cmd_cli_rm(int const,
			   const char *const);
int		cmd_cli_sys(int const,
			    const char *const);
int		cmd_cli_lsys(int const,
			     const char *const);
int		cmd_cli_rmdir(int const,
			      const char *const);
int		cmd_cli_lrmdir(int const,
			       const char *const);
int		cmd_cli_clear(int const,
			      const char *const);
int		cmd_cli_mkdir(int const,
			      const char *const);
int		cmd_cli_lmkdir(int const,
			       const char *const);
int		cmd_cli_get(int const,
			    const char *const);
int		cmd_cli_set(int const,
			    const char *const);
int		cmd_cli_pwd(int const,
			    const char *const);
int		cmd_cli_quit(int const,
			     const char *const);
int		cmd_cli_cd(int const,
			   const char *const);
int		cmd_cli_ls(int const,
			   const char *const);
int		cmd_cli_lls(int const,
			    const char *const);
int		cmd_cli_lcd(int const,
			    const char *const);
int		cmd_cli_lpwd(int const,
			     const char *const);
int		client_logout(int const);
int		client_login(int const);
int		handle_arg(char *, int const, int, int);

typedef struct	s_arg
{
  char		*str;
  int		(*fct)(int const, const char *const);
}		t_arg;

#endif /* !CLIENT_H_ */
