#define _GNU_SOURCE
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "shared.h"
#include "server.h"

static void	tricky(char *print, unsigned int *s)
{
  print[strlen(print) + 1] = 0;
  print[strlen(print)] = '\n';
  *s = strlen(print);
}

static char	*list_to_send(unsigned int s, char *print)
{
  DIR		*dir;
  struct dirent	*tmp;

  if ((dir = opendir(".")) == NULL)
    {
      my_perror(__func__, __LINE__, __FILE__, FAILURE);
      return (NULL);
    }
  while ((tmp = readdir(dir)) != NULL)
    {
      if ((print = realloc(print, s + strlen(tmp->d_name) + 2)) == NULL)
	{
	  my_perror(__func__, __LINE__ - 2, __FILE__, FAILURE);
	  return (NULL);
	}
      strcpy(print + s, tmp->d_name);
      tricky(print, &s);
    }
  if (closedir(dir) == -1)
    {
      my_perror(__func__, __LINE__ - 2, __FILE__, FAILURE);
      return (NULL);
    }
  return (print);
}

int		handle_ls(char *buffer UNUSED,
			  int const fd,
			  t_infos *infos UNUSED)
{
  char		*print;
  char		*data;

  if ((print = list_to_send(0, NULL)) == NULL)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  if (asprintf(&data, "%s%s", CMD_OK, print) == -1)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, data, strlen(data));
  memset(buffer, 0, 4097);
  read(fd, buffer, 4096);
  printf("%s\n", buffer);
  if (strncmp(buffer, END_DATA_TR, strlen(END_DATA_TR)) != 0)
    return (client_logout(fd));
  return (SUCCESS);
}
