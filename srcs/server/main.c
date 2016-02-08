#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "server.h"
#include "shared.h"

static int		follow_connection(int const client_fd,
					  t_infos *infos)
{
  int			i;

  if (chdir("user") == -1)
    if (mkdir("user", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1
	|| chdir("user") == -1)
    return (client_logout(client_fd));
  i = -1;
  while (infos->user_name[++i])
    if (infos->user_name[i] == '\r' || infos->user_name[i] == '\n')
      infos->user_name[i] = 0;
  if (chdir(infos->user_name) == -1)
    {
      if (mkdir(infos->user_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1
	  || chdir(infos->user_name) == -1)
	return (client_logout(client_fd));
    }
  write(client_fd, USER_LOGGED_IN, strlen(USER_LOGGED_IN));
  return (SUCCESS);
}

static int		client_login(int const client_fd,
				     t_infos *infos)
{
  char			buffer[4097];
  int			tmp;

  memset(buffer, 0, 4097);
  write(client_fd, SERV_READY_NEW_USER, strlen(SERV_READY_NEW_USER));
  if ((tmp = read(client_fd, buffer, 4096)) <= 0)
    return (client_logout(client_fd));
  if ((strncmp("USER Anonymous\r\n", buffer, 17) == 0)
      || strncmp("USER anonymous\r\n", buffer, 17) == 0)
    {
      if ((infos->user_name = strdup("anonymous")) == NULL)
	return (client_logout(client_fd));
      return (follow_connection(client_fd, infos));
    }
  if (strlen(buffer) < 6 || ((infos->user_name = strdup(buffer + 5)) == NULL))
    return (client_logout(client_fd));
  write(client_fd, USER_OK_NEED_PSW, strlen(USER_OK_NEED_PSW));
  if (read(client_fd, buffer, 4096) <= 0)
    return (client_logout(client_fd));
  return (follow_connection(client_fd, infos));
}

static void		serveur_for_client(int const client_fd)
{
  char			buffer[4097];
  t_infos		infos;

  client_login(client_fd, &infos);
  if (((infos.user_path = strdup("/")) == NULL) ||
      ((infos.binary_path = getcwd(NULL, 0)) == NULL))
    {
      my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE);
      client_logout(client_fd);
    }
  memset(buffer, 0, 4097);
  while (read(client_fd, buffer, 4096) > 0 &&
	 handle_command(buffer, client_fd, &infos) != FAILURE)
    memset(buffer, 0, 4097);
  client_logout(client_fd);
}

static int		handle_server(int const connection_fd)
{
  int			client_fd;
  struct sockaddr_in	sock_client;
  socklen_t		size;
  char			*client_ip;

  size = sizeof(struct sockaddr_in);
  if (listen(connection_fd, MAX_CLIENT) == -1)
    return (print_failure(NULL));
  while (42)
    {
      client_fd = -1;
      if ((client_fd = accept(connection_fd, (struct sockaddr *)
			      &sock_client, &size)) == -1)
	return (print_failure(NULL));
      if (client_fd != -1 && fork() == 0)
	serveur_for_client(client_fd);
      client_ip = inet_ntoa(sock_client.sin_addr);
      printf("%s\n", client_ip);
    }
  return (SUCCESS);
}

int			main(int const ac __attribute__((unused)),
			     char const *const ag[])
{
  struct protoent	*proto;
  struct sockaddr_in	in;
  int			connection_fd;

  if (check_nbr(ag[1]) == FAILURE)
    return (print_basic_failure(ARG_ERROR));
  if ((proto = getprotobyname("TCP")) == NULL)
    return (print_failure(NULL));
  if ((connection_fd = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
    return (print_failure(NULL));
  in.sin_family = AF_INET;
  in.sin_port = (in_port_t)htons((uint16_t)(atoi(ag[1])));
  in.sin_addr.s_addr = INADDR_ANY;
  if (bind(connection_fd, (const struct sockaddr *)&in, sizeof(in)) == -1)
    {
      print_failure(NULL);
      if (close(connection_fd) == -1)
	return (print_failure(NULL));
      return (FAILURE);
    }
  handle_server(connection_fd);
  if (close(connection_fd) == -1)
    return (print_failure(NULL));
  return (SUCCESS);
}
