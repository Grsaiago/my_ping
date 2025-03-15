#include "../include/my_ping.h"
#include <netinet/ip_icmp.h>

static void	print_generic_ok(ProgramConf *conf, IcmpReply *message);
static void	print_time_exceded_reply(ProgramConf *conf, IcmpReply *message);
static void print_ip_icmp_hdr(struct iphdr *iphdr, struct icmp *icmp);
static void	calculate_footer_metrics(PrintMetrics *metrics, const PingPacketStats *stats);
static double	calculate_avg(const PingPacketStats *stats);
static uint32_t	calculate_packet_loss_percent(const PingPacketStats *stats);
static double	calculate_stdandard_deviation(const PingPacketStats *stats);

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
	PrintMetrics	print_metrics;

	calculate_footer_metrics(&print_metrics, &conf->pkt_stats);
	printf("--- %s statistics --- \n", conf->program_arg);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
		conf->pkt_stats.oks + conf->pkt_stats.errors, conf->pkt_stats.oks, print_metrics.loss_percent);
	if (print_metrics.loss_percent != 100) {
		 /*round-trip min/avg/max/stddev = 9.163/9.226/9.296/0.055 ms */
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
			conf->pkt_stats.rtt_min, print_metrics.avg,
			conf->pkt_stats.rtt_max, print_metrics.stddev);
	}
	return ;
}

void	print_icmp_message(ProgramConf *conf, IcmpReply *message, IcmpMessage *original_message) {
	switch (message->icmp.icmp_type) {
		case (ICMP_ECHO):
		case (ICMP_ECHOREPLY):
			print_generic_ok(conf, message);
			break;
		case (ICMP_TIMXCEED):
			print_time_exceded_reply(conf, message);
			if (conf->flags.verbose == true) {
				print_ip_icmp_hdr(&message->iphdr, &original_message->icmp);
			}
			break;
		default:
			printf("Received unsupported icmp message\n");
			break;
	}
	return ;
}

static void	print_generic_ok(ProgramConf *conf, IcmpReply *message) {
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.4f ms\n",
		ntohs(message->iphdr.tot_len),
		conf->resolved_addr, message->icmp.icmp_hun.ih_idseq.icd_seq,
		message->iphdr.ttl, calculate_rtt_in_ms(&message->sent_at, &message->recv_at));
	return ;
}

// No support for ipv6, only ipv4
static void	print_time_exceded_reply(ProgramConf *conf, IcmpReply *message) {
	char		resolved_addr[INET6_ADDRSTRLEN] = {0};
	struct in_addr ipv4;

	ipv4.s_addr = message->iphdr.saddr;
	inet_ntop(conf->ip_version, &(ipv4.s_addr),
	     resolved_addr, sizeof(resolved_addr));
	printf("%d bytes from (%s): Time to live exceeded\n",
		ntohs(message->iphdr.tot_len), resolved_addr);
	return ;
}

static void calculate_footer_metrics(PrintMetrics *metrics, const PingPacketStats *stats) {
	metrics->avg = calculate_avg(stats);
	metrics->loss_percent = calculate_packet_loss_percent(stats);
	metrics->stddev = calculate_stdandard_deviation(stats);
	return ;
};

static double calculate_avg(const PingPacketStats *stats) {
	uint32_t	total_packets;

	total_packets = stats->oks + stats->errors;
	if (total_packets == 0 || stats->rtt_sum == 0) {
		return (0);
	} else {
		return (stats->rtt_sum / total_packets);
	}
}

static uint32_t	calculate_packet_loss_percent(const PingPacketStats *stats) {
	if (stats->oks != 0 && stats->errors != 0) {
		return ((uint32_t)(100 * stats->errors) / (stats->oks + stats->errors));
	} else if (stats->oks == 0 && stats->errors != 0) {
		return (100);
	} else {
		return (0);
	}
}

static double calculate_stdandard_deviation(const PingPacketStats *stats) {
	double mean = stats->rtt_sum / stats->oks;
	double variance = (stats->rtt_sum_square / stats->oks) - (mean * mean);

	if (stats->oks == 0) {
		return 0.0f;
	}
	mean = stats->rtt_sum / stats->oks;
	variance = (stats->rtt_sum_square / stats->oks) - (mean * mean);
	return sqrt(variance);
}

// Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data
//  4  5  00 0054 5d02   2 0000  01  01 4ee5 172.26.11.30  142.251.135.142
// ICMP: type 8, code 0, size 64, id 0x082d, seq 0x0001

static void print_ip_icmp_hdr(struct iphdr *iphdr, struct icmp *icmp) {
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
    printf(" %1x  %1x  %02x %04x %04x   %1x %04x  %02x  %02x %04x %s  %s\n",
           iphdr->version, iphdr->ihl, iphdr->tos, ntohs(iphdr->tot_len), ntohs(iphdr->id),
           (ntohs(iphdr->frag_off) & 0xE000) >> 13, ntohs(iphdr->frag_off) & 0x1FFF,
           iphdr->ttl, iphdr->protocol, ntohs(iphdr->check),
           inet_ntoa(*(struct in_addr *)&iphdr->saddr),
           inet_ntoa(*(struct in_addr *)&iphdr->daddr));
	   printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
           icmp->icmp_type, icmp->icmp_code,
           ntohs(iphdr->tot_len) - (iphdr->ihl * 4),
		   ntohs(icmp->icmp_hun.ih_idseq.icd_id),
           ntohs(icmp->icmp_hun.ih_idseq.icd_seq));
	return ;
}
