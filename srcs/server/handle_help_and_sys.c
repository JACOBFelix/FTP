#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "client.h"

int			handle_sys(char *str UNUSED,
				   int const fd,
				   t_infos *infos UNUSED)
{
  char			*data;
  struct utsname	name;

  if (uname(&name) == -1)
    {
      my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE);
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  if (asprintf(&data, "%s%s\n%s\n%s\n%s\n", CMD_OK, name.sysname,
	       name.nodename, name.release, name.machine) == -1
      || strlen(data) > 4096)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, data, strlen(data));
  free(data);
  return (SUCCESS);
}
