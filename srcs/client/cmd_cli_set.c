#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "shared.h"

static int	follow_cmd_cli_set(int const fd,
				   int const fd_file)
{
  char		buf[4097];
  char		size[5];
  char		*data;
  int		tmp;

  while (1)
    {
      memset(buf, 0, 4097);
      if ((tmp = read(fd_file, buf, 4096)) <= 0
	  || asprintf(&data, "%d", tmp) == -1)
	{
	  write(fd, "0000", 4);
	  break ;
	}
      memset(size, 0, 5);
      strcpy(size, data);
      write(fd, size, 4);
      write(fd, buf, tmp);
    }
  write(fd, END_DATA_TR, strlen(END_DATA_TR));
  read(fd, buf, 4096);
  if (close(fd_file) == -1)
    my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS);
  return (SUCCESS);
}

int		cmd_cli_set(int const socket_fd,
			    const char *const str)
{
  char		buffer[4097];
  char		*data;
  int		fd;

  printf("%s\n", __func__);
  if (str == NULL || str[0] == 0 || asprintf(&data, "STOR %s\r\n", str) == -1
      || (fd = open(str, O_RDONLY)) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  write(socket_fd, data, strlen(data));
  memset(buffer, 0, 4097);
  read(socket_fd, buffer, 4096);
  if (strncmp(buffer, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ)) == 0)
    return (SUCCESS);
  write(socket_fd, OPEN_DATA, strlen(OPEN_DATA));
  return (follow_cmd_cli_set(socket_fd, fd));
}
