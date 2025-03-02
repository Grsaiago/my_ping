#include "../include/my_ping.h"
int main(int argc, char *argv[]) {

	(void)argc;
	ProgramConf conf;
	initialize_program_conf(&conf);
	// etapa de flags:
	// validar as flags
	// etapa de dns resolution:
	validate_or_resolve_address(&conf, argv[1]);
	// Usar o getaddrinfo pra fazer o lookup/validar o endereço que a pessoa mandou
	// usar esse valor da etapa de dns resolution pra criar a struct PingProgram
	// separar duas threads, uma pra mandar de 1 em 1 s o ping
	// outra pra ler o que for recebido e fazer o processamento disso.

	return EXIT_SUCCESS;
}
