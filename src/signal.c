#include "../include/my_ping.h"

static void	signal_handler(int sig);

void	install_signal_handlers(void) {
	struct sigaction sig_conf = {0};

	sig_conf.sa_handler = signal_handler;

	sigaction(SIGINT, &sig_conf, NULL);
}

void	signal_handler(int sig) {
	extern bool my_ping_should_continue;

	(void)sig;
	my_ping_should_continue = false;

	return;
}
