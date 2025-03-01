#ifndef MY_PING
# define MY_PING

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

/* socket */
typedef struct s_RawSocket {
	int	fd;
}	RawSocket;

/* functions */
int	new_raw_socket(void);

#endif // !MY_PING
