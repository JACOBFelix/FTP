#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include "client.h"
#include "shared.h"

int			cmd_cli_lls(int const socket_fd UNUSED,
			    const char *const str UNUSED)
{
  DIR			*dir;
  struct dirent		*tmp;

  if ((dir = opendir(".")) == NULL)
    {
      perror(__func__);
      return (SUCCESS);
    }
  while ((tmp = readdir(dir)) != NULL)
    printf("%s\n", tmp->d_name);
  if (closedir(dir) == -1)
    my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS);
  return (SUCCESS);
}

int		cmd_cli_lcd(int const socket_fd UNUSED,
			    const char *const str)
{
  if (str == NULL || str[0] == 0)
    return (SUCCESS);
  if (chdir(str) == -1)
    my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS);
  return (SUCCESS);
}

int		cmd_cli_lpwd(int const socket_fd UNUSED,
			     const char *const str UNUSED)
{
  char  	*data;

  if ((data = getcwd(NULL, 0)) == NULL)
    {
      printf("Don't know where we are\n");
      return (SUCCESS);
    }
  printf("%s\n", data);
  free(data);
  return (SUCCESS);
}
