#include "../include/my_ping.h"
#include <netinet/ip_icmp.h>

static void record_time_exceded_message(ProgramConf *conf, IcmpMessage *message);
static void record_echo_reply_message(ProgramConf *conf, IcmpMessage *message);

void record_new_message(ProgramConf *conf, IcmpMessage *message) {
	switch (message->icmp.icmp_type) {
		case (ICMP_ECHOREPLY):
			record_echo_reply_message(conf, message);
			break;
		case (ICMP_TIME_EXCEEDED):
			record_time_exceded_message(conf, message);
			break;
		default:
			printf("Tried to record unsupported message, recording it as an error\n");
			break ;
	}
	return ;
}

static void record_echo_reply_message(ProgramConf *conf, IcmpMessage *message) {
	double	packet_rtt;

	packet_rtt = calculate_rtt_in_ms(&message->sent_at, &message->recv_at);
	conf->pkt_stats.oks++;
	conf->pkt_stats.rtt_sum += packet_rtt;
	if (conf->pkt_stats.rtt_min > packet_rtt) {
		conf->pkt_stats.rtt_min = packet_rtt;
	}
	if (conf->pkt_stats.rtt_max < packet_rtt) {
		conf->pkt_stats.rtt_max = packet_rtt;
	}
	return ;
}

static void record_time_exceded_message(ProgramConf *conf, IcmpMessage *message) {
	(void)message;
	conf->pkt_stats.errors++;
	return ;
}
