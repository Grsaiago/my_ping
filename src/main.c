#include "../include/my_ping.h"

int main(int argc, char *argv[]) {
	struct sockaddr_storage socket_confs = {0};
	ProgramConf conf;

	install_signal_handlers();
	initialize_program_conf(&conf);
	parse_arguments(&conf, argc, argv);
	/*printf("O valor das flags é:\nverbose: %d\nso_debug: %d\nlinger: %d\npacket_interval: %d\ncount: %d\n", conf.flags.verbose, conf.flags.so_debug, conf.flags.linger, conf.flags.packet_interval, conf.flags.count);*/
	return (0);
	if (validate_or_resolve_address(&conf, argv[1], (struct sockaddr *)&socket_confs) == -1) {
		return (-1);
	}
	if (new_raw_socket(&conf.main_socket, &socket_confs, &conf.flags) == -1) {
		return (-1);
	}
	event_loop(&conf);
	return (EXIT_SUCCESS);
}

int	event_loop(ProgramConf *conf) {
	struct icmp	message = {0};
	struct timeval	start;
	struct timeval	end;
	int		err_value = 0;

	while(true) {
		// if it didn't error_create a new message.
		// If there was an error, we need to retry the same message.
		if (!err_value) {
			message = new_icmp_echo_message(conf);
		}
		gettimeofday(&start, NULL);
		err_value = send_icmp_message(&conf->main_socket, message);
		if (!err_value) {
			recv_icmp_message(&conf->main_socket);
		} else {
			continue;
		}
		gettimeofday(&end, NULL);
		printf("the latency for the message is: %ld ms\n", ((end.tv_sec * (uint64_t)1000) + (end.tv_usec / 1000)) - ((start.tv_sec * (uint64_t)1000) + (start.tv_usec / 1000)));
		sleep(conf->flags.packet_interval);
	}
	return (0);
}
