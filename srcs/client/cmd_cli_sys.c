#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "client.h"

int			cmd_cli_lsys(int const fd UNUSED,
				    const char *const str UNUSED)
{
  char			*data;
  struct utsname	name;

  if (uname(&name) == -1)
    return (my_perror(__func__, __LINE__ - 1, __FILE__, SUCCESS));
  if (asprintf(&data, "%s\n%s\n%s\n%s\n", name.sysname,
	       name.nodename, name.release, name.machine) == -1
      || strlen(data) > 4096)
    return (SUCCESS);
  printf("%s", data);
  free(data);
  return (SUCCESS);
}

int		cmd_cli_sys(int const fd UNUSED,
			    const char *const STR UNUSED)
{
  char			buffer[4097];

  printf("%s\n", __func__);
  memset(buffer, 0, 4097);
  write(fd, "SYS\r\n", 5);
  read(fd, buffer, 4096);
  printf("%s", buffer);
  return (SUCCESS);
}
