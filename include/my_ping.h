#ifndef MY_PING
# define MY_PING

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <unistd.h>

typedef enum e_IpVersion {
	IPV4 = AF_INET,
	IPV6 = AF_INET6,
}	IpVersion;

/* socket */
typedef struct s_Socket {
	int			fd;
	struct sockaddr_storage	remote_addr;
}	Socket;

/* ping program */
typedef struct s_ProgramConf {
	bool		is_verbose;
	IpVersion	ip_version;
	Socket		main_socket;
}	ProgramConf;

/* functions */
int	new_raw_socket(struct sockaddr_storage *remote_addr, Socket *res);
void	initialize_program_conf(ProgramConf *conf);
// Performs dns lookup/ip validation using getaddrinfo
int	validate_or_resolve_address(ProgramConf *program_conf, char *address);

#endif // !MY_PING
