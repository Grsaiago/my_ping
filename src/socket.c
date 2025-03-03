#include "../include/my_ping.h"

int	new_raw_socket(Socket *res, struct sockaddr_storage *remote_addr, ExecutionFlags *flags) {
	int	sock;
	int	value;

	// we don't need a raw socket because:
	// https://sturmflut.github.io/linux/ubuntu/2015/01/17/unprivileged-icmp-sockets-on-linux/
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock == -1) {
		dprintf(STDERR_FILENO, "error on socket creation: %s\n", strerror(errno));
		return (-1);
	}
	if (flags->so_debug == true) {
		value = 1;
		setsockopt(sock, SOL_SOCKET, SO_DEBUG, &value, sizeof(int));
	}
	if (flags->linger > 0) {
		struct timeval	linger_time = {
			.tv_sec = flags->linger,
			.tv_usec = 0,
		};
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &linger_time, sizeof(struct timeval));
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
