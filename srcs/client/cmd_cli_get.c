#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"
#include "shared.h"

int		follow_cmd_cli_get(int const socket_fd,
				   int fd)
{
  char		buf_size[5];
  char		buffer[4097];
  int		size;

  size = 1;
  while (size != 0)
    {
      memset(buf_size, 0, 5);
      read(socket_fd, buf_size, 4);
      size = atoi(buf_size);
      if (size == 0)
	break ;
      read(socket_fd, buffer, size);
      write(fd, buffer, size);
    }
  memset(buffer, 0, 4097);
  read(socket_fd, buffer, 4096);
  printf("%s", buffer);
  close(fd);
  return (SUCCESS);
}

int		cmd_cli_get(int const socket_fd,
			    const char *const str)
{
  char		buffer[4097];
  char		*data;
  int		fd;

  if (str == NULL || str[0] == 0 || asprintf(&data, "RETR %s\n", str) == -1
      || (fd = creat(str, O_RDWR | S_IRUSR | S_IWUSR | S_IRGRP)) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  write(socket_fd, data, strlen(data));
  memset(buffer, 0, 4097);
  read(socket_fd, buffer, 4096);
  if (strncmp(buffer, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ)) == 0)
    {
      printf("%s", buffer);
      return (SUCCESS);
    }
  write(socket_fd, OPEN_DATA, strlen(OPEN_DATA));
  return (follow_cmd_cli_get(socket_fd, fd));
}
