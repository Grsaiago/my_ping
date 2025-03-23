#include "../include/my_ping.h"

unsigned short calculate_checksum(void *b, int len) {
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;
	for (sum = 0; len > 1; len -= 2) {
		sum += *buf++;
	}
	if (len == 1) {
		sum += *(unsigned char *)buf;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

IcmpMessage	new_icmp_echo_message(ProgramConf *conf) {
	IcmpMessage message = {
		.icmp = {
			.icmp_type = ICMP_ECHO,
			.icmp_code = 0,
			.icmp_hun.ih_idseq = {
				.icd_id = getpid(),
				.icd_seq = ++conf->msg_seq,
			}
		}
	};
	gettimeofday(&message.timestamp, NULL);
	message.icmp.icmp_cksum = calculate_checksum(&message, sizeof(IcmpMessage));
	return (message);
}

int	send_icmp_message(Socket *sock, IcmpMessage *message) {
	int		err_value;
	err_value = sendto(sock->fd, (void *)message, sizeof(IcmpMessage), 0,
		    (const struct sockaddr *)&sock->remote_addr,
		    sock->addr_struct_size);
	if (err_value < (int)sock->addr_struct_size) {
		if (err_value == -1) {
			dprintf(STDERR_FILENO, "erro no sendto: %s\n", strerror(errno));
		} else {
			dprintf(STDERR_FILENO, "erro no sendto: de %ld bytes, somente %d foram enviados", sizeof(struct icmp), err_value);
		}
		return (1);
	}
	return (0);
}

int	recv_icmp_message(Socket *sock, IcmpReply *message) {
	// as per man raw: "For receiving, the IP header is always included in the packet."
	char		readbuff[sizeof(struct iphdr) + sizeof(IcmpMessage)] = {0};
	int		err_value;

	// in the case a person runs two pings on the machine
	while (message->icmp.icmp_hun.ih_idseq.icd_id != getpid()) {
		err_value = recvfrom(sock->fd, readbuff, sizeof(readbuff),
		       0, NULL, NULL);
		if (err_value < (int)sock->addr_struct_size) {
			if (err_value == -1) {
				dprintf(STDERR_FILENO, "erro no recvfrom: %s\n", strerror(errno));
				return (1);
			} else {
				dprintf(STDERR_FILENO, "erro no recvfrom: de %ld bytes, somente %d foram lidos", sizeof(struct icmp), err_value);
				return (1);
			}
		}
		message->iphdr = *(struct iphdr *)readbuff;
		message->icmp = *(struct icmp *)(readbuff + (message->iphdr.ihl * 4));
	}
	return (0);
}

double	calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end) {
	double	latency;

	latency = ((end->tv_sec * (double)1000) + ((double)end->tv_usec / 1000))
		- ((start->tv_sec * (double)1000) + ((double)start->tv_usec / 1000));
	return (latency);
}
