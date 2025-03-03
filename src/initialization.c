#include "../include/my_ping.h"

void	initialize_program_conf(ProgramConf *conf) {
	memset(conf, 0, sizeof(ProgramConf));
	conf->ip_version = IPV4;
	conf->flags.packet_interval = 1;
	conf->flags.linger = -1;
}
