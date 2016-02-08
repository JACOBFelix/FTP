#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "client.h"
#include "shared.h"

int			follow_handle_arg(int const socket_fd,
					  char *buffer,
					  int i,
					  int a)
{
  static const t_arg	arg[20] = {{"lsys", &cmd_cli_lsys},
				   {"sys", &cmd_cli_sys},
				   {"ls", &cmd_cli_ls},
				   {"cd", &cmd_cli_cd},
				   {"set", &cmd_cli_set},
				   {"get", &cmd_cli_get},
				   {"put", &cmd_cli_set},
				   {"pwd", &cmd_cli_pwd},
				   {"quit", &cmd_cli_quit},
				   {"lls", &cmd_cli_lls},
				   {"lcd", &cmd_cli_lcd},
				   {"lpwd", &cmd_cli_lpwd},
				   {"rm", &cmd_cli_rm},
				   {"mkdir", &cmd_cli_mkdir},
				   {"lmkdir", &cmd_cli_lmkdir},
				   {"clear", &cmd_cli_clear},
				   {"rmdir", &cmd_cli_rmdir},
				   {"lrmdir", &cmd_cli_lrmdir},
				   {"mget", &cmd_cli_mget},
				   {"mput", &cmd_cli_mset}};

  while (++a < 20)
    if (strncmp(arg[a].str, buffer + i, strlen(arg[a].str)) == 0)
      return (arg[a].fct(socket_fd, buffer + i + strlen(arg[a].str) + 1));
  return (SUCCESS);
}

int	        	handle_arg(char *buffer,
				   int const socket_fd,
				   int i,
				   int a)
{
  while (buffer[++i])
    if (buffer[i] == '\n')
      buffer[i] = 0;
  i = -1;
  while (buffer[++i] == ' ' || buffer[i] == '\t')
    ;
  if (buffer[i] == 0)
    return (SUCCESS);
  return (follow_handle_arg(socket_fd, buffer, i, a));
}

static int	follow_client_login(int const socket_fd,
				    char buffer[],
				    int tmp,
				    char *data)
{
  if (strncmp(buffer, USER_LOGGED_IN, strlen(USER_LOGGED_IN)) == 0)
    return ((printf((LOG_ANO)) ^ strlen(LOG_ANO)) + SUCCESS);
  printf(ASK_PASS);
  if ((tmp = read(0, buffer, 4096)) <= 0)
    return (client_logout(socket_fd));
  if (tmp > 1 && buffer[tmp - 1] == '\n')
    tmp--;
  buffer[tmp] = 0;
  if (((tmp = asprintf(&data, "PASS %s\r\n", buffer)) == -1)
      || write(socket_fd, data, strlen(data)) == -1
      || memset(buffer, 0, 4097) != buffer
      || read(socket_fd, buffer, 4096) <= 0
      || strncmp(buffer, USER_LOGGED_IN, strlen(USER_LOGGED_IN) != 0))
    return (client_logout(socket_fd));
  free(data);
  return (SUCCESS);
}

int		client_login(int const socket_fd)
{
  char		buffer[4097];
  int		tmp;
  char		*data;

  memset(buffer, 0, 4097);
  if (((tmp = read(socket_fd, buffer, 4096)) <= 0) ||
      strncmp(buffer, SERV_READY_NEW_USER, strlen(SERV_READY_NEW_USER)) != 0)
    return (client_logout(socket_fd));
  if (printf(WHAT_NAME) != WHAT_NAM_LEN || (tmp = read(0, buffer, 4096)) <= 0)
    return (client_logout(socket_fd));
  if (tmp > 1 && buffer[tmp - 1] == '\n')
    tmp = tmp - 1;
  buffer[tmp] = 0;
  if (((tmp = asprintf(&data, "%s%s\r\n", "USER ", buffer)) == -1)
    || write(socket_fd, data, strlen(data)) != (ssize_t)strlen(data))
    return (client_logout(socket_fd));
  free(data);
  memset(buffer, 0, 4097);
  if ((tmp = read(socket_fd, buffer, 4096)) <= 0)
    return (client_logout(socket_fd));
  return (follow_client_login(socket_fd, buffer, tmp, data));
}
