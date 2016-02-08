#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared.h"
#include "client.h"

int		cmd_cli_mget(int const socket_fd,
				     const char *const str)
{
  char			*tmp;
  char			*token;

  if ((tmp = strdup(str)) == NULL)
    {
      fprintf(stderr,
	      "Error allocation memory, can't send request to server\n");
      return (my_perror(__func__, __LINE__ - 3, __FILE__, SUCCESS));
    }
  token = strtok(tmp, " \t");
  while (token != NULL)
    {
      cmd_cli_get(socket_fd, token);
      token = strtok(NULL, " \t");
    }
  return (SUCCESS);
}

int		cmd_cli_mset(int const socket_fd,
				     const char *const str)
{
  char			*tmp;
  char			*token;

  if ((tmp = strdup(str)) == NULL)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  token = strtok(tmp, " \t");
  while (token != NULL)
    {
      cmd_cli_set(socket_fd, token);
      token = strtok(NULL, " \t");
    }
  return (SUCCESS);
}
