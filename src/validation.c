#include "../include/my_ping.h"

int	validate_or_resolve_address(ProgramConf *conf, char *address, struct sockaddr *res) {
	struct addrinfo	*getaddr_result;
	struct addrinfo	getaddr_hints;
	int		error_value;

	getaddr_result = NULL;
	// fill hints struct
	memset(&getaddr_hints, 0, sizeof(struct addrinfo));
	getaddr_hints.ai_family = conf->ip_version; // since they're renamings of either AF_INET or AF_INET6
	getaddr_hints.ai_socktype = 0; // any socket type
	getaddr_hints.ai_protocol = 0; // any protocol
	
	// call getaddrinfo and get the found/validated address and feed it to program_conf
	error_value = getaddrinfo(address, NULL, &getaddr_hints, &getaddr_result);
	if (error_value != 0) {
		dprintf(STDERR_FILENO, "error: %s\n", gai_strerror(error_value));
		return (-1);
	}
	memcpy(res, getaddr_result->ai_addr, getaddr_result->ai_addrlen);
	freeaddrinfo(getaddr_result);
	return (0);
}
