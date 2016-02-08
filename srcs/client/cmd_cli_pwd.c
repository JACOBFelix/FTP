#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "client.h"
#include "shared.h"

int		cmd_cli_pwd(int const socket_fd,
			      const char *const str UNUSED)
{
  char		buffer[4097];

  write(socket_fd, "PWD", 3);
  memset(buffer, 0, 4097);
  if (read(socket_fd, buffer, 4096) <= 0)
    return (client_logout(socket_fd));
  if (strncmp(buffer, PATH_CREAT, strlen(PATH_CREAT)) != 0)
    return (client_logout(socket_fd));
  printf("%s", buffer);
  return (SUCCESS);
}
