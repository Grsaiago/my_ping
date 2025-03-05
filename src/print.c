#include "../include/my_ping.h"

void	print_header(ProgramConf *conf) {
	printf("PING %s (%s): %ld data bytes\n",
		conf->program_arg, conf->resolved_addr,
		sizeof(struct iphdr) + sizeof(struct icmp));
}

void	print_footer(ProgramConf *conf) {
	printf("--- %s statistics --- \n", conf->program_arg);
	printf("TODO uma porrada de estatística\n");
	return ;
}

void	print_icmp_message(ProgramConf *conf, IcmpMessage *message) {
	double	latency;

	latency = ((message->recv_at.tv_sec * (double)1000) + ((double)message->recv_at.tv_usec / 1000))
		- ((message->sent_at.tv_sec * (double)1000) + ((double)message->sent_at.tv_usec / 1000));
	printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.4f ms\n",
		sizeof(struct icmp) + sizeof(struct iphdr),
		conf->resolved_addr, message->icmp.icmp_hun.ih_idseq.icd_seq,
		message->iphdr.ttl, latency);
	return ;
}
