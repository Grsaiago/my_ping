#include "../include/my_ping.h"

static void record_time_exceded_response(ProgramConf *conf, IcmpReply *message);
static void record_echo_reply_response(ProgramConf *conf, IcmpReply *message);

void record_new_response(ProgramConf *conf, IcmpReply *message) {
	switch (message->icmp.icmp_type) {
		case (ICMP_ECHOREPLY):
			record_echo_reply_response(conf, message);
			break;
		case (ICMP_TIME_EXCEEDED):
			record_time_exceded_response(conf, message);
			break;
		default:
			printf("Tried to record unsupported message, recording it as an error\n");
			break ;
	}
	return ;
}

static void record_echo_reply_response(ProgramConf *conf, IcmpReply *message) {
	double	packet_rtt;

	packet_rtt = calculate_rtt_in_ms(&message->sent_at, &message->recv_at);
	if (conf->pkt_stats.oks == 0 || conf->pkt_stats.rtt_min > packet_rtt) {
		conf->pkt_stats.rtt_min = packet_rtt;
	}
	if (conf->pkt_stats.oks == 0 || conf->pkt_stats.rtt_max < packet_rtt) {
		conf->pkt_stats.rtt_max = packet_rtt;
	}

	conf->pkt_stats.oks++;
	conf->pkt_stats.rtt_sum += packet_rtt;
	conf->pkt_stats.rtt_sum_square += packet_rtt * packet_rtt;
	return ;
}

static void record_time_exceded_response(ProgramConf *conf, IcmpReply *message) {
	(void)message;
	conf->pkt_stats.errors++;
	return ;
}
