#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "shared.h"

int		my_perror(char const *const func,
			  int const line,
			  char const *const file,
			  int const ret)
{
  fprintf(stderr, "error in function : %s\nline : %d\nfile : %s\n",
	  func, line, file);
  perror(NULL);
  return (ret);
}

int	check_nbr(char const *const str)
{
  int	i;

  if (!str)
    return (FAILURE);
  i = -1;
  while (str[++i])
    if (str[i] < '0' || str[i] > '9')
      return (FAILURE);
  return (SUCCESS);
}

int	print_basic_failure(char const *const str)
{
  write(2, str, strlen(str));
  return (FAILURE);
}

int	print_failure(char const *const str)
{
  perror(str);
  return (FAILURE);
}

int	print_failure_and_close(int const fd, char const *const str)
{
  perror(str);
  if (close(fd) == -1)
    perror(NULL);
  return (FAILURE);
}
