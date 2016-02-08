#ifndef SERVER_H_
# define SERVER_H_

# define ARG_ERROR	"./server [ID Port]\n"
# define MAX_CLIENT	42

typedef struct	s_infos
{
  char		*user_name;
  char		*user_path;
  char		*binary_path;
}		t_infos;
typedef struct	s_arg
{
  char		*str;
  int		length;
  int		(*fct)(char *, int const, t_infos *);
}		t_arg;

int		client_logout(int const);
int		handle_sys(char *, int const, t_infos *);
int		handle_noop(char *, int const, t_infos *);
int		handle_quit(char *, int const, t_infos *);
int		handle_set(char *, int const, t_infos *);
int		handle_get(char *, int const, t_infos *);
int		handle_ls(char *, int const, t_infos *);
int		handle_pass(char *, int const, t_infos *);
int		handle_user(char *, int const, t_infos *);
int		handle_pwd(char *, int const, t_infos *);
int		handle_not_found(char *, int const, t_infos *);
int		handle_cdup(char *, int const, t_infos *);
int		handle_cd(char *, int const, t_infos *);
int		handle_command(char *, int const, t_infos *);
int		handle_rm(char *, int const, t_infos *);

#endif /* !SERVER_H_ */
