#include "../include/my_ping.h"
#include <netinet/in.h>

int	new_raw_socket(struct sockaddr_storage *remote_addr, Socket *res) {
	int sock;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock == -1) {
		return (-1);
	}
	res->remote_addr = *remote_addr;
	res->fd = sock;
	return (0);
}
