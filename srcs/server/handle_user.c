#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "server.h"

int		handle_not_found(char *buffer UNUSED,
				int const fd,
				t_infos *infos UNUSED)
{
  write(fd, CMD_NT_FND, strlen(CMD_NT_FND));
  return (SUCCESS);
}

int		handle_pass(char *buffer,
			    int const fd,
			    t_infos *infos UNUSED)
{
  int		i;

  if (infos->user_name == NULL)
    return (write(fd, NEED_ACOUNT, strlen(NEED_ACOUNT)) - 1 || SUCCESS);
  i = 4;
  if (buffer[i] != ' ')
    {
      write(fd, USER_OK_NEED_PSW, strlen(USER_OK_NEED_PSW));
      return (SUCCESS);
    }
  while (buffer[++i] != '\r' && buffer[i])
    ;
  if (buffer[i] != '\r' || buffer[i + 1] != '\n')
    {
      write(fd, USER_OK_NEED_PSW, strlen(USER_OK_NEED_PSW));
      return (SUCCESS);
    }
  write(fd, USER_LOGGED_IN, strlen(USER_LOGGED_IN));
  return (SUCCESS);
}

int		handle_user(char *buffer,
			    int const fd,
			    t_infos *infos)
{
  int		i;

  i = 4;
  if (strncmp(buffer, "USER Anonymous\r\n", 16) == 0)
    {
      if ((infos->user_name = strdup("Anonymous")) == NULL)
	{
	  my_perror(__func__, __LINE__ - 2, __FILE__, SUCCESS);
	  return (write(fd, CLOS_DATA_CO, strlen(CLOS_DATA_CO)) - 1 && FAILURE);
	}
      write(fd, USER_LOGGED_IN, strlen(USER_LOGGED_IN));
      return (SUCCESS);
    }
  if (buffer[i] != ' ')
    return (write(fd, NEED_ACOUNT, strlen(NEED_ACOUNT)) - 1 || SUCCESS);
  while (buffer[++i] != '\r' && buffer[i] != 0)
    ;
  if (buffer[i] == 0 || buffer[i + 1] != '\n')
    return (write(fd, NEED_ACOUNT, strlen(NEED_ACOUNT)) || SUCCESS);
  buffer[i] = 0;
  if ((infos->user_name = strdup(buffer + 5)) == NULL)
    return (write(fd, CLOS_DATA_CO, strlen(CLOS_DATA_CO)) && FAILURE);
  return (write(fd, USER_OK_NEED_PSW, strlen(USER_OK_NEED_PSW)) || SUCCESS);
}

int		handle_quit(char *buffer UNUSED,
			    int const fd,
			    t_infos *infos UNUSED)
{
  client_logout(fd);
  return (FAILURE);
}

int		handle_noop(char *buffer UNUSED,
			    int const fd,
			    t_infos *infos UNUSED)
{
  write(fd, CMD_OK, strlen(CMD_OK));
  return (SUCCESS);
}
