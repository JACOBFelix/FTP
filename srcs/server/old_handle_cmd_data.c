#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "shared.h"
#include "server.h"

static int	follow_handle_set(int const socket_fd,
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
      memset(buffer, 0, 4097);
      read(socket_fd, buffer, size);
      write(fd, buffer, size);
    }
  memset(buffer, 0, 4097);
  read(socket_fd, buffer, 4096);
  printf("%s", buffer);
  if (close(fd) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  return (SUCCESS);
}

int		handle_set(char *buffer,
			   int const fd,
			   t_infos *infos UNUSED)
{
  int		fd_file;
  int		i;
  char		bufr[4097];

  i = 5;
  while (buffer[++i] && buffer[i] != '\n' && buffer[i] != '\r')
    ;
  buffer[i] = 0;
  if ((fd_file = creat(buffer + 5, O_RDWR
		       | S_IRUSR | S_IWUSR | S_IRGRP)) == -1)
    {
      write(fd, BAD_ARG_CMD, strlen(BAD_ARG_CMD));
      return (SUCCESS);
    }
  write(fd, CMD_OK, strlen(CMD_OK));
  memset(bufr, 0, 4097);
  read(fd, bufr, 4096);
  write(1, bufr, strlen(buffer));
  return (follow_handle_set(fd, fd_file));
}

static int	  follow_handle_get(int const fd,
				    int fd_file)
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
  return (SUCCESS);
}

int		handle_get(char *buffer,
			   int const fd,
			   t_infos *infos UNUSED)
{
  int		fd_file;
  char		buf[4096];

  if (buffer == NULL || buffer[5] == 0)
    {
      write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
      return (SUCCESS);
    }
  if (buffer[strlen(buffer) - 1] == '\n')
    buffer[strlen(buffer) - 1] = 0;
  if ((fd_file = open(buffer + 5, O_RDONLY)) == -1)
       {
	 my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE);
	 write(fd, BAD_CMD_SEQ, strlen(BAD_CMD_SEQ));
	 return (SUCCESS);
       }
  write(fd, OPEN_DATA, strlen(OPEN_DATA));
  read(fd, buf, strlen(OPEN_DATA));
  return (follow_handle_get(fd, fd_file));
}
