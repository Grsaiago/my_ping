#include "../include/my_ping.h"

void	initialize_program_conf(ProgramConf *conf) {
	memset(conf, 0, sizeof(ProgramConf));
	conf->continue_execution = true;
	conf->ip_version = IPV4;
	conf->msg_seq = -1;
	conf->flags.packet_interval = 1;
	conf->flags.linger = -1;
	conf->flags.count = -1;
	conf->flags.ttl = DEFAULT_TTL;
	conf->pkt_stats.rtt_max = DBL_MIN;
	conf->pkt_stats.rtt_min = DBL_MAX;
}
