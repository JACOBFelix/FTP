#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "client.h"
#include "shared.h"

int		cmd_cli_rm(int const socket_fd,
			   const char *const str)
{
  char		buffer[4097];
  char		*data;

  if (asprintf(&data, "DELE %s\r\n", str) == -1)
    {
      printf("Can't send request to server\n");
      return (SUCCESS);
    }
  memset(buffer, 0, 4097);
  if (write(socket_fd, data, strlen(data)) == -1
      || read(socket_fd, buffer, 4096) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE));
  printf("%s", buffer);
  free(data);
  return (SUCCESS);
}
