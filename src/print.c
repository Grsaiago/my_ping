#include "../include/my_ping.h"

static void	print_echo_reply_ok(ProgramConf *conf, IcmpMessage *message);
static double	calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end);
static void	print_time_exceded_reply(ProgramConf *conf, IcmpMessage *message);

void	print_header(ProgramConf *conf) {
	if (conf->flags.verbose) {
		int pid = getpid();
		printf("PING %s (%s): %ld data bytes, id %#x = %d\n",
			conf->program_arg, conf->resolved_addr,
			sizeof(struct iphdr) + sizeof(struct icmp), pid, pid);
	} else {
		printf("PING %s (%s): %ld data bytes\n",
			conf->program_arg, conf->resolved_addr,
			sizeof(struct iphdr) + sizeof(struct icmp));
	}
}

void	print_footer(ProgramConf *conf) {
	printf("--- %s statistics --- \n", conf->program_arg);
	printf("TODO uma porrada de estatística\n");
	return ;
}

void	print_icmp_message(ProgramConf *conf, IcmpMessage *message) {
	switch (message->icmp.icmp_type) {
		case (ICMP_ECHOREPLY):
			print_echo_reply_ok(conf, message);
			break;
		case (ICMP_TIMXCEED):
			print_time_exceded_reply(conf, message);
			break;
		default:
			printf("Received unsupported icmp message\n");
			break;
	}
	return ;
}

static void	print_echo_reply_ok(ProgramConf *conf, IcmpMessage *message) {
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.4f ms\n",
		ntohs(message->iphdr.tot_len),
		conf->resolved_addr, message->icmp.icmp_hun.ih_idseq.icd_seq,
		message->iphdr.ttl, calculate_rtt_in_ms(&message->sent_at, &message->recv_at));
	return ;
}

// No support for ipv6, only ipv4
static void	print_time_exceded_reply(ProgramConf *conf, IcmpMessage *message) {
	char		resolved_addr[INET6_ADDRSTRLEN] = {0};
	struct in_addr ipv4;


	ipv4.s_addr = message->iphdr.saddr;
	inet_ntop(conf->ip_version, &(ipv4.s_addr),
	     resolved_addr, sizeof(resolved_addr));
	printf("%d bytes from (%s): Time to live exceeded\n",
		ntohs(message->iphdr.tot_len), resolved_addr);
	return ;
}

static double	calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end) {
	double	latency;

	latency = ((end->tv_sec * (double)1000) + ((double)end->tv_usec / 1000))
		- ((start->tv_sec * (double)1000) + ((double)start->tv_usec / 1000));
	return (latency);
}
