#include <string.h>
#include <unistd.h>
#include "shared.h"

int		cmd_cli_clear(int const fd UNUSED,
			      const char *const str UNUSED)
{
  write(1, "\033[H\033[2J", strlen("\033[H\033[2J"));
  return (SUCCESS);
}
