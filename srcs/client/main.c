#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "shared.h"

static int	check_ip(const char *const str)
{
  int		i;
  int		a;

  if (strcmp(str, INVALID_ADDR) == 0)
    return (print_basic_failure(INVALID_IP));
  i = -1;
  a = 0;
  while (str[++i])
    {
      if (str[i] == '.')
	++a;
      else if (str[i] < '0' || str[i] > '9')
	return (print_basic_failure(NOT_A_IP));
    }
  if (a != 3)
    return (print_basic_failure(NOT_A_IP));
  return (SUCCESS);
}

static int	print_prompt()
{
  write(1, PROMPT, strlen(PROMPT));
  return (SUCCESS);
}

static int	start_client(int const socket_fd)
{
  char		*buffer;
  size_t	buf_size;

  if (client_login(socket_fd) == FAILURE)
    return (FAILURE);
  buffer = NULL;
  buf_size = 0;
  printf("client_login success\n");
  while (print_prompt() == SUCCESS
	 && getline(&buffer, &buf_size, stdin) != -1
	 && handle_arg(buffer, socket_fd, -1, -1) == SUCCESS)
    {
      buf_size = 0;
      free(buffer);
    }
  cmd_cli_quit(socket_fd, NULL);
  free(buffer);
  return (SUCCESS);
}

int			main(int const ac, char const *const ag[])
{
  struct protoent	*proto;
  struct sockaddr_in	in;
  int			socket_fd;

  if (ac < 3 || (check_nbr(ag[2]) == FAILURE) || (check_ip(ag[1]) == FAILURE))
    return (print_basic_failure(CLI_ARG_ERROR));
  if ((proto = getprotobyname("TCP")) == NULL)
    return (print_basic_failure(GETPROTOBYNAME_ERR));
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE));
  in.sin_family = AF_INET;
  in.sin_port = (in_port_t)htons((uint16_t)atoi(ag[2]));
  in.sin_addr.s_addr = inet_addr(ag[1]);
  if (connect(socket_fd, (struct sockaddr *)&in, sizeof(in)) == -1)
    {
      my_perror(__func__, __LINE__ - 2, __FILE__, SUCCESS);
      if (close(socket_fd) == -1)
	return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
      return (FAILURE);
    }
  start_client(socket_fd);
  if (close(socket_fd) == -1)
    return (print_failure(NULL));
  return (0);
}
