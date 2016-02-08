#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shared.h"

static int	my_write(int fd,
			 char *str,
			 char buffer[],
			 int i)
{
  write(fd, str, strlen(str));
  write(fd, ":", 1);
  write(fd, buffer, i);
  write(fd, "\n", 1);
  printf("DataBase Uploaded\n");
  return (SUCCESS);
}

static int	add_user(char *str UNUSED, int fd UNUSED)
{
  char		buffer[4097];
  int		i;

  printf("Enter password for %s\n", str);
  memset(buffer, 0, 4097);
  if (read(0, buffer, 4096) < 1)
    return (my_perror(__func__, __LINE__ - 3, __FILE__, FAILURE));
  if (strlen(buffer) < 9)
    {
      printf("Password need almost 8 characters\n");
      return (my_perror(__func__, __LINE__ - 3, __FILE__, FAILURE));
    }
  i = -1;
  while (buffer[++i])
    buffer[i] ^= '\n';
  if (mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    return (my_perror(__func__, __LINE__ - 3, __FILE__, FAILURE));
  return (my_write(fd, str, buffer, i));
}

static int	open_repo()
{
  if (chdir("user") == -1)
    {
      if (mkdir("user", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
	return (my_perror(__func__, __LINE__ - 3, __FILE__, FAILURE));
    }
  return (SUCCESS);
}

static int	my_end(int const fd)
{
  if (close(fd) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, FAILURE));
  return (SUCCESS);
}

int	main(int ac, char **ag)
{
  int	i;
  int	fd;

  if (ac < 2)
    {
      fprintf(stderr, "Usage ./add_user [USER_NAME]\n");
      return (FAILURE);
    }
  if ((fd = open("user_ftp.db", O_RDWR)) == -1)
    return (my_perror(__func__, __LINE__ - 3, __FILE__, FAILURE));
  if (open_repo() == FAILURE)
    return (FAILURE);
  i = 0;
  while (++i < ac)
    if (add_user(ag[i], fd) == SUCCESS)
      printf("User %s created\n", ag[i]);
  return (my_end(fd));
}
