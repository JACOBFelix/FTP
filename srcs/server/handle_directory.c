#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shared.h"
#include "server.h"

int		handle_cdup(char *buffer UNUSED,
			    int const fd,
			    t_infos *infos)
{
  int		i;
  int		c;

  printf("%s\n", __func__);
  if (strcmp(infos->user_path, "/") == 0)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  i = -1;
  c = 0;
  while (infos->user_path[++i])
    if (infos->user_path[i] == '/' && infos->user_path[i + 1] != 0)
      c = i;
  if (c == 0)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  infos->user_path[c] = 0;
  write(fd, CMD_OK, strlen(CMD_OK));
  return (SUCCESS);
}

static int	check_right_direc(char *buffer,
				  t_infos *infos,
				  int const fd,
				  char *cwd)
{
  char		*bu;

  if (((cwd = getcwd(NULL, 0)) == NULL) || chdir(buffer) == -1)
    return (FAILURE);
  if ((bu = getcwd(NULL, 0)) == NULL)
    {
      if (chdir(infos->binary_path) == -1)
	return (client_logout(fd));
      return (FAILURE);
    }
  if (strncmp(infos->binary_path, bu, strlen(infos->binary_path)) == 0)
    {
      free(infos->user_path);
      if ((infos->user_path
	   = strdup(bu + strlen(infos->binary_path))) == NULL)
	return (client_logout(fd));
      if (infos->user_path[0] == 0
	  && (infos->user_path = strdup("/")) == NULL)
	return (client_logout(fd));
      return (SUCCESS);
    }
  else if (chdir(infos->binary_path) == -1)
    client_logout(fd);
  free(cwd);
  return (SUCCESS);
}

int		handle_cd(char *buffer,
			  int const fd,
			  t_infos *infos)
{
  int		i;

  printf("%s\n%s\n", __func__, buffer);
  i = -1;
  while (buffer[++i] && buffer[i] != '\n' && buffer[i] != '\r')
    ;
  buffer[i] = 0;
  if (buffer == NULL || buffer[0] == 0 || strlen(buffer) < 5)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  if (check_right_direc(buffer + 4, infos, fd, NULL) == FAILURE)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, REQ_OK, strlen(REQ_OK));
  return (SUCCESS);
}

int		handle_pwd(char *buffer UNUSED,
			   int const fd,
			   t_infos *infos)
{
  char		*data;

  if (asprintf(&data, "%s%s\r\n", PATH_CREAT, infos->user_path) == -1)
    return (client_logout(fd));
  write(fd, data, strlen(data));
  free(data);
  return (SUCCESS);
}
