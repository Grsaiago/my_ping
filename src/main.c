#include "../include/my_ping.h"

int main(int argc, char *argv[]) {
	struct sockaddr_storage socket_confs = {0};
	ProgramConf conf;
	(void)argc;

	// TODO: Install signal handler for SIGINT to end loop
	// TODO: Parse das flags em uma estrutura e passar essa estrutura pro init program confs
	// ou fazer o parse das flags na função mesmo.
	initialize_program_conf(&conf);
	if (validate_or_resolve_address(&conf, argv[1], (struct sockaddr *)&socket_confs) == -1) {
		return (-1);
	}
	if (new_raw_socket(&conf.main_socket, &socket_confs, &conf.flags) == -1) {
		return (-1);
	}
	event_loop(&conf);
	return (EXIT_SUCCESS);
}
