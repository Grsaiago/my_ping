#include "../include/my_ping.h"

int	validate_or_resolve_address(ProgramConf *conf, char *address) {
	struct addrinfo	*getaddr_result;
	struct addrinfo	getaddr_hints;
	int		error_value;
	// debug declarations
	struct sockaddr_in *ipv4_addr;
	struct sockaddr_in6 *ipv6_addr;
	const char *result;

	getaddr_result = NULL;
	// fill hints struct
	memset(&getaddr_hints, 0, sizeof(struct addrinfo));
	getaddr_hints.ai_family = conf->ip_version; // since they're renamings of either AF_INET or AF_INET6
	getaddr_hints.ai_socktype = 0; // any socket type
	getaddr_hints.ai_protocol = 0; // any protocol
	
	// call getaddrinfo and get the found/validated address and feed it to program_conf
	error_value = getaddrinfo(address, NULL, &getaddr_hints, &getaddr_result);
	if (error_value != 0) {
		dprintf(STDERR_FILENO, "my_ping: error on dns lookup at getaddrinfo. %s\n", gai_strerror(error_value));
		return (-1);
	}
	if (getaddr_result == NULL) {
		dprintf(STDERR_FILENO, "my_ping: unknown host\n");
		freeaddrinfo(getaddr_result);
		return (-1);
	}
	memcpy(&conf->main_socket.remote_addr, getaddr_result->ai_addr, getaddr_result->ai_addrlen);
	char	buff[100] = {0};
	switch (conf->ip_version) {
		case (IPV4):
			ipv4_addr = (struct sockaddr_in *)&conf->main_socket.remote_addr;
			result = inet_ntop(AF_INET, &ipv4_addr->sin_addr, buff, sizeof(buff));
			break;
		case (IPV6):
			ipv6_addr = (struct sockaddr_in6 *)&conf->main_socket.remote_addr;
			result = inet_ntop(AF_INET6, &ipv6_addr->sin6_addr, buff, sizeof(buff));
			break;
	}
	(void)result;
	printf("The ip is [%s]", buff);
	freeaddrinfo(getaddr_result);
	return (0);
}
