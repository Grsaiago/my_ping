#include "../include/my_ping.h"

int main(int argc, char *argv[]) {
	struct sockaddr_storage socket_confs = {0};

	(void)argc;
	ProgramConf conf;
	initialize_program_conf(&conf);
	// etapa de flags:
	// validar as flags
	// etapa de dns resolution:
	if (validate_or_resolve_address(&conf, argv[1], (struct sockaddr *)&socket_confs) == -1) {
		return (-1);
	}
	if (new_raw_socket(&conf.main_socket, &socket_confs) == -1) {
		return (-1);
	}
	// Usar o getaddrinfo pra fazer o lookup/validar o endereço que a pessoa mandou
	// usar esse valor da etapa de dns resolution pra criar a struct PingProgram
	// separar duas threads, uma pra mandar de 1 em 1 s o ping
	// outra pra ler o que for recebido e fazer o processamento disso.
	struct icmp message = {
		.icmp_type = ICMP_ECHO,
		.icmp_code = 0,
		.icmp_hun.ih_idseq = {
			.icd_id = getpid(),
			.icd_seq = 1,
		}
	};
	message.icmp_cksum = calculate_checksum(&message, sizeof(message));
	send_icmp_message(&conf.main_socket, message);
	return EXIT_SUCCESS;
}
