#ifndef MY_PING
# define MY_PING

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

/* ip packet */
typedef struct s_IpHeader {
	int		version : 4;
	int		ihl : 4;
	uint8_t		type_of_service;
	uint16_t	total_lenght;
	uint16_t	identification;
	int		flags : 3;
	int		fragment_offset : 13;
	uint8_t		time_to_live;
	uint8_t		protocol;
	uint16_t	header_checksum;
	uint32_t	source_address;
	uint32_t	destination_address;
	uint32_t	options_padding;
}	IpHeader;

/* icmp messages */
typedef enum e_IcmpMessageType {
	DESTINATION_UNREACHABLE = 3
}	IcmpMessageType;

typedef struct s_IcmpDestinationUnreachableMessage {
	uint8_t type;
}	IcmpDestinationUnreachableMessage;

typedef union u_IcmpMessagePayload {
	IcmpDestinationUnreachableMessage destination_unreachable;
}	IcmpMessagePayload;

typedef struct s_IcmpMessage {
	IcmpMessageType		m_type;
	IcmpMessagePayload	m_payload;
} IcmpMessage;

typedef struct s_IcmpPacket {
	IpHeader	header;
	IcmpMessage	payload;
}	IcmpPacket;

/* socket */
typedef struct s_RawSocket {
	int	fd;
}	RawSocket;

/* functions */
int	new_raw_socket(void);

#endif // !MY_PING
