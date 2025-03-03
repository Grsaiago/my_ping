#ifndef MY_PING
# define MY_PING

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <unistd.h>
# include <errno.h>

typedef enum e_IpVersion {
	IPV4 = AF_INET,
	IPV6 = AF_INET6,
}	IpVersion;

/* socket */
typedef struct s_Socket {
	int			fd;
	struct sockaddr_storage	remote_addr;
	struct sockaddr_in	*ipv4_addr;
	struct sockaddr_in6	*ipv6_addr;
	socklen_t		addr_struct_size;
}	Socket;

/* ping program */
typedef struct s_ProgramConf {
	bool		is_verbose;
	IpVersion	ip_version;
	Socket		main_socket;
}	ProgramConf;

/* functions */
int	new_raw_socket(Socket *res, struct sockaddr_storage *remote_addr);
void	initialize_program_conf(ProgramConf *conf);
// Performs dns lookup/ip validation for address using getaddrinfo and places found sockaddr_* in struct res
int	validate_or_resolve_address(ProgramConf *conf, char *address, struct sockaddr *res);
/* icmp messaging */
unsigned short	calculate_checksum(void *b, int len);
int		send_icmp_message(Socket *sock, struct icmp message);

#endif // !MY_PING
