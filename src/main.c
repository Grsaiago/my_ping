#include "../include/my_ping.h"

bool my_ping_should_continue = true;

int main(int argc, char *argv[]) {
	struct sockaddr_storage socket_confs = {0};
	ProgramConf conf;

	initialize_program_conf(&conf);
	parse_arguments(&conf, argc, argv);
	if (conf.flags.verbose) {
		printf("O valor das flags é:\nverbose: %d\nso_debug: %d\nlinger: %d\npacket_interval: %d\ncount: %d\nO valor da struct é:\narg: %s\n", conf.flags.verbose, conf.flags.so_debug, conf.flags.linger, conf.flags.packet_interval, conf.flags.count, conf.program_arg);
	}
	install_signal_handlers();
	if (validate_or_resolve_address(&conf, (struct sockaddr *)&socket_confs) == -1) {
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
	extern bool	my_ping_should_continue;
	int		err_value = 0;
	int		recv_result = 0;

	print_header(conf);
	while(my_ping_should_continue == true) {
		// if it didn't error_create a new message.
		// If there was an error, we need to retry the same message.
		if (!err_value) {
			message = new_icmp_echo_message(conf);
		}
		gettimeofday(&start, NULL);
		err_value = send_icmp_message(&conf->main_socket, message);
		if (!err_value) {
			recv_result = recv_icmp_message(&conf->main_socket);
			if (recv_result == -1 && errno == EINTR) {
				break;
			}
		} else {
			continue;
		}
		gettimeofday(&end, NULL);
		printf("the latency for the message is: %ld ms\n", ((end.tv_sec * (uint64_t)1000) + (end.tv_usec / 1000)) - ((start.tv_sec * (uint64_t)1000) + (start.tv_usec / 1000)));
		sleep(conf->flags.packet_interval);
	}
	printf("Saiu do event loop em\n");
	return (0);
}
