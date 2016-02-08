#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "shared.h"
#include "client.h"

int		cmd_cli_lmkdir(int const socket_fd UNUSED,
			       const char *const str)
{
  if (str[0] == 0
      || mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE));
  return (SUCCESS);
}

int		cmd_cli_mkdir(int const socket_fd,
			      const char *const str)
{
  char		buffer[4097];
  char		*data;

  if (asprintf(&data, "MKD %s\r\n", str) == -1)
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

int		cmd_cli_rmdir(int const socket_fd,
			      const char *const str)
{
  char		buffer[4097];
  char		*data;

  if (asprintf(&data, "RMD %s\r\n", str) == -1)
    {
      printf("Can't send request to server\n");
      return (SUCCESS);
    }
  if (write(socket_fd, data, strlen(data)) == -1
      || read(socket_fd, buffer, 4096) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE));
  printf("%s", buffer);
  free(data);
  return (SUCCESS);
}

int		cmd_cli_lrmdir(int const socket_fd UNUSED,
			       const char *const str)
{
  if (str[0] == 0
      || rmdir(str) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  printf("%s has been removed\n", str);
  return (SUCCESS);
}
