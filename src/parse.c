#include "../include/my_ping.h"

static error_t	parser_func(int key, char *arg, struct argp_state *state);
static int32_t	set_linger(ProgramConf *conf, char *str_nbr);
static int32_t	set_interval(ProgramConf *conf, char *str_nbr);
static int32_t	set_count(ProgramConf *conf, char *str_nbr);

// argp global variables as per: https://www.gnu.org/software/libc/manual/html_node/Argp-Global-Variables.html
const char			*argp_program_version = "1.0";

int	parse_arguments(ProgramConf *conf, int argc, char *argv[]) {
	static struct argp_option	options[] = {
		{ .name = "count", .key = 'c', .arg = "NUMBER", .doc = "Stop after sending NUMBER packets" },
		{ .name = "debug", .key = 'd', .arg = NULL, .doc = "Set the SO_DEBUG option" },
		{ .name = "interval", .key = 'i', .arg = "NUMBER", .doc = "Wait NUMBER seconds between sending each packet" },
		{ .name = "verbose", .key = 'v', .arg = NULL, .doc = "Verbose output" },
		{ .name = "linger", .key = 'W', .arg = "N", .doc = "Number of seconds to wait for response" },
		{0}
	};

	const struct argp argp = {
		.options = options,
		.parser = parser_func,
		.args_doc = "[HOST]",
		.doc = "Send ICMP ECHO_REQUEST packets to network hosts.",
	};
	argp_parse(&argp, argc, argv, 0, 0, (void *)conf);
	return (0);
}

static error_t	parser_func(int key, char *arg, struct argp_state *state) {
	(void)arg;
	ProgramConf	*conf;

	conf = (ProgramConf *)state->input;
	switch (key) {
		case ('c'):
			if (arg == NULL || set_count(conf, arg) == -1) {
				argp_error(state, "Count cannot be less than 0\n");
			}
			break ;
		case ('d'):
			conf->flags.so_debug= true;
			break ;
		case ('i'):
			if (arg == NULL || set_interval(conf, arg) == -1) {
				argp_error(state, "Interval cannot be less than 1\n");
			}
			break ;
		case ('v'):
			conf->flags.verbose = true;
			break ;
		case ('W'):
			if (arg == NULL || set_linger(conf, arg) == -1) {
				argp_error(state, "Linger cannot be less than 0\n");
			}
			break ;
		case (ARGP_KEY_ARG):
			if (state->arg_num >= 1) {
				argp_usage(state);
			}
			conf->program_arg = arg;
			break;
		case (ARGP_KEY_END):
			if (state->arg_num < 1) {
				argp_usage(state);
			}
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

static int32_t	set_count(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL,0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.count = result_nbr;
	return (0);
}

static int32_t	set_interval(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL,0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.packet_interval = result_nbr;
	return (0);
}

static int32_t	set_linger(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL,0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.linger = result_nbr;
	return (0);
}
