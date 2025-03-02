#include <stdint.h>
#include <sys/types.h>

typedef enum e_MessageType {
	DESTINATION_UNREACHABLE = 3
}	MessageType;

typedef struct s_DestinationUnreachableMessage {
	uint8_t type;
}	DestinationUnreachableMessage;

typedef union u_MessagePayload {
	DestinationUnreachableMessage destination_unreachable;
}	MessagePayload;

struct Message {
	MessageType	m_type;
	MessagePayload	m_payload;
};
