#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"
#include "shared.h"

static int	cmd_cli_cd_up(int const socket_fd UNUSED)
{
  char	        buffer[4097];

  write(socket_fd, "CDUP", 4);
  memset(buffer, 0, 4097);
  if (read(socket_fd, buffer, 4096) < 1)
    return (client_logout(socket_fd));
  write(1, buffer, strlen(buffer));
  return (SUCCESS);
}

int		cmd_cli_cd(int const socket_fd,
			   const char *const str)
{
  char	        buffer[4097];
  int		tmp;
  char		*data;

  if (str == NULL || str[0] == 0)
    return (cmd_cli_cd_up(socket_fd));
  if (asprintf(&data, "CWD %s\r\n", str) == -1)
    return (client_logout(socket_fd));
  write(socket_fd, data, strlen(data));
  memset(buffer, 0, 4097);
  if ((tmp = read(socket_fd, buffer, 4096)) <= 0)
    return (client_logout(socket_fd));
  printf("%s", buffer);
  return (SUCCESS);
}
