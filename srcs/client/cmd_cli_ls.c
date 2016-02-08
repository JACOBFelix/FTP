#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "shared.h"

int		cmd_cli_ls(int const socket_fd,
			   const char *const str UNUSED)
{
  char		buffer[4096 * 4 + 1];
  int		tmp;

  write(socket_fd, "LIST", 4);
  memset(buffer, 0, 4096 * 4 + 1);
  if ((tmp = read(socket_fd, buffer, 4096 * 4)) <= 0)
    {
      printf("%s\n", buffer);
      return (client_logout(socket_fd));
    }
  printf("%s", buffer);
  if (strncmp(buffer, CMD_OK, strlen(CMD_OK)) != 0)
    return (SUCCESS);
  memset(buffer, 0, 4096 * 4 + 1);
  if (write(socket_fd, END_DATA_TR, strlen(END_DATA_TR)) == -1)
    return (client_logout(socket_fd));
  return (SUCCESS);
}
