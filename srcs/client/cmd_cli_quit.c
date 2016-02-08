#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "client.h"
#include "shared.h"

int	client_logout(int const socket_fd)
{
  write(socket_fd, CLIENT_ASK_QUIT, strlen(CLIENT_ASK_QUIT));
  if (close(socket_fd) == -1)
    my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE);
  exit(0);
  return (FAILURE);
}

int		cmd_cli_quit(int const socket_fd,
			     const char *const str UNUSED)
{
  printf("Good Bye\n");
  return (client_logout(socket_fd));
}
