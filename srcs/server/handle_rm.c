#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shared.h"
#include "server.h"

int		handle_rm(char *buffer,
			  int const fd,
			  t_infos *infos UNUSED)
{
  int		i;

  i = -1;
  while (buffer[++i] && buffer[i] != '\r' && buffer[i] != '\n')
    ;
  buffer[i] = 0;
  if (buffer == NULL || buffer[0] == 0 || strlen(buffer) < 6
      || unlink(buffer + 5) == -1)
    {
      my_perror(__func__, __LINE__ - 2, __FILE__, SUCCESS);
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  write(fd, REQ_OK, strlen(REQ_OK));
  return (SUCCESS);
}
