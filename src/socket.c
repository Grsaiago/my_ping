#include "../include/my_ping.h"
#include <netinet/in.h>

int	new_raw_socket(Socket *res, struct sockaddr_storage *remote_addr) {
	int sock;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock == -1) {
		dprintf(STDERR_FILENO, "error on socket creation: %s\n", strerror(errno));
		return (-1);
	}
	res->fd = sock;
	res->remote_addr = *remote_addr;
	switch (res->remote_addr.ss_family) {
		case (IPV4):
			res->ipv4_addr = (struct sockaddr_in *)&res->remote_addr;
			res->addr_struct_size = sizeof(struct sockaddr_in);
			break;
		case (IPV6):
			res->ipv6_addr = (struct sockaddr_in6 *)&res->remote_addr;
			res->addr_struct_size = sizeof(struct sockaddr_in6);
			break; 

	}
	return (0);
}
