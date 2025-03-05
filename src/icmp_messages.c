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

struct icmp	new_icmp_echo_message(ProgramConf *conf) {
	struct icmp message = {
		.icmp_type = ICMP_ECHO,
		.icmp_code = 0,
		.icmp_hun.ih_idseq = {
			.icd_id = getpid(),
			.icd_seq = ++conf->msg_seq,
		}
	};
	message.icmp_cksum = calculate_checksum(&message, sizeof(struct icmp));
	return (message);
}

int	send_icmp_message(Socket *sock, struct icmp message) {
	int		err_value;
	err_value = sendto(sock->fd, (void *)&message, sizeof(struct icmp), 0,
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

int	recv_icmp_message(Socket *sock, IcmpMessage *message) {
	// as per man raw: "For receiving, the IP header is always included in the packet."
	char		readbuff[sizeof(struct icmp) + sizeof(struct iphdr)] = {0};
	int		err_value;

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
	return (0);
}
