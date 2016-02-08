#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "shared.h"
#include "server.h"

static int	handle_mkdir(char *buffer,
			     int const fd,
			     t_infos *infos UNUSED)
{
  int		i;

  i = -1;
  while (buffer[++i] == ' ' || buffer[i] == '\t')
    ;
  buffer += i;
  i = -1;
  while (buffer[++i] && buffer[i] != '\n' && buffer[i] != '\r')
    ;
  buffer[i] = 0;
  if (buffer[0] == 0 || strlen(buffer) < 4
      || mkdir(buffer + 4, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, CMD_OK, strlen(CMD_OK));
  return (SUCCESS);
}

static int	handle_rmdir(char *buffer,
			     int const fd,
			     t_infos *infos UNUSED)
{
  int		i;

  i = -1;
  while (buffer[++i] == ' ' || buffer[i] == '\t')
    ;
  buffer += i;
  i = -1;
  while (buffer[++i] && buffer[i] != '\n' && buffer[i] != '\r')
    ;
  buffer[i] = 0;
  if (buffer[0] == 0 || strlen(buffer) < 4
      || rmdir(buffer + 4) == -1)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, CMD_OK, strlen(CMD_OK));
  return (SUCCESS);
}

int		client_logout(int const client_fd)
{
  write(client_fd, CLOS_DATA_CO, strlen(CLOS_DATA_CO));
  if (close(client_fd) == -1)
    perror(NULL);
  printf("Client Logout\n");
  exit(0);
  return (FAILURE);
}

int			handle_command(char *buffer,
				       int const client_fd,
				       t_infos *infos)
{
  int			i;
  static const t_arg	arg[15] = {{"USER", 4, &handle_user},
				   {"PASS", 4, &handle_pass},
				   {"CWD", 3, &handle_cd},
				   {"PWD", 3, &handle_pwd},
				   {"CDUP", 4, &handle_cdup},
				   {"QUIT", 4, &handle_quit},
				   {"LIST", 4, &handle_ls},
				   {"EPSV", 4, &handle_ls},
				   {"RETR", 4, &handle_get},
				   {"STOR", 4, &handle_set},
 				   {"NOOP", 4, &handle_noop},
				   {"DELE", 4, &handle_rm},
				   {"MKD", 3, &handle_mkdir},
				   {"RMD", 3, &handle_rmdir},
				   {"SYS", 3, &handle_sys}};

  i = -1;
  while (++i < 15)
    if (strncmp(buffer, arg[i].str, arg[i].length) == 0)
      return (arg[i].fct(buffer, client_fd, infos));
  return (handle_not_found(buffer, client_fd, infos));
}

