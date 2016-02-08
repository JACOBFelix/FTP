#ifndef SHARED_H_
# define SHARED_H_

# define SUCCESS		1
# define FAILURE		0
# define GETPROTOBYNAME_ERR	"Error : getprotobyname failed\n"
# define CMD_NT_FND "202 Command not implemented, superflous at this sit.\r\n"
# define CMD_NOT_IMP_SUPERFLOUS "202 Command not implemented, superfluous at\
this site."
# define OPEN_DATA "150 File status okay; about to open data connection.\r\n"
# define CMD_OK "200 Command okay.\r\n"
# define SERV_READY_NEW_USER "220 Service ready for new user.\r\n"
# define S_R_N_U SERV_READY_NEW_USER
# define CLOS_DATA_CO "221 Service closing control connection.\
Logged out if appropriate.\r\n"
# define END_DATA_TR "226 Closing data connection.\
Requested file action successful (for example, file\
transfer or file abort)."
# define PING_OK "Received\n"
# define USER_LOGGED_IN "230 User logged in, proceed\r\n"
# define REQ_OK "250 Requested file action okay, completed.\r\n"
# define PATH_CREAT "257 \"PATHNAME\" created.\r\n"
# define CLIENT_ASK_QUIT "QUIT\r\n"
# define USER_OK_NEED_PSW "331 User name okay, need password.\r\n"
# define NEED_ACOUNT "332 Need account for login.\r\n"
# define BAD_ARG_CMD "501 Syntax Error in parameters or arguments.\r\n"
# define BAD_CMD_SEQ "503 Bad sequence of commands.\r\n"
# define PING_KO "Not Received\n"
# define UNUSED		__attribute__((unused))

int	check_nbr(char const *const str);
int	print_failure(char const *const str);
int     print_basic_failure(char const *const str);
int	print_failure_and_close(int const fd, char const *const str);
int	my_perror(char const *const, int const, char const *const, int const);

#endif /* !SHARED_H_ */
