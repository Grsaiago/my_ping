# ft_ping

Recreating the inettools-2.0 ping in C, one ICMP ECHO_REQUEST packet at a time xD

## 📂 Project Structure

```sh
.
├── Makefile                # Makefile for building and managing the project
├── README.md               # This file
├── include/                # Header files
│   └── my_ping.h           # Main header file for my_ping
├── obj/                    # Compiled object files (generated during build)
└── src/                    # Source code files
    ├── icmp_messages.c     # Handles ICMP message creation and parsing
    ├── initialization.c    # Initializes the ping utility
    ├── main.c              # Entry point of the program
    ├── message_record.c    # Tracks sent and received messages
    ├── parse.c             # Parses command-line arguments
    ├── print.c             # Handles output and logging
    ├── signal.c            # Manages signal handling (e.g., Ctrl+C)
    ├── socket.c            # Manages socket creation and communication
    └── validation.c        # Validates input and network parameters
```

## 🛠️ Features

- GNU-compliant CLI: Uses parse_argp to provide a standard command-line interface.

- Cli Options:

```sh
-c, --count=NUMBER         Stop after sending NUMBER packets
-d, --debug                Set the SO_DEBUG option
-i, --interval=NUMBER      Wait NUMBER seconds between sending each packet
-t, --ttl=N                specify N as time-to-live
-v, --verbose              Verbose output
-W, --linger=N             Number of seconds to wait for response
-?, --help                 Give this help list
--usage                Give a short usage message
-V, --version              Print program version
```

- ICMP Protocol Implementation: Handles ICMP ECHO_REQUEST,
ECHO_REPLY, and ICMP_TIMXCEED messages.

- Signal Handling: Gracefully handles interruptions (e.g., Ctrl+C).

- Final metrics: min, max and average round trip time (rtt) measurement,
as well as standard deviation.

## Examples

- Ping a host 5 times:

```sh
sudo ./my_ping -c 5 google.com
```

- Set a custom interval between packets

```sh
sudo ./my_ping -i 2 google.com
```

- Enable verbose output

```sh
sudo ./my_ping -v google.com
```

## How It Works

### Initialization:

1. The program parses command-line arguments and validates the target host.
1. A raw socket is created to send and receive ICMP packets.
1. The hostname is resolved with DNS or validated (in case of an ipaddr input).

### ICMP Packet Handling:

1. ICMP ECHO_REQUEST packets are constructed and sent to the target host.

1. The program listens for ECHO_REPLY packets and calculates round-trip times.

### Signal Handling:

1. The program handles interruptions (e.g., Ctrl+C) and displays statistics before exiting.

### Output:

1. The program prints detailed information about each packet,
including sequence numbers, round-trip times, and TTL values.

## References

### RFCS

1. [Ip Packet Definition](https://datatracker.ietf.org/doc/html/rfc791#section-3.1)
1. [ICMP RFC](https://datatracker.ietf.org/doc/html/rfc792)

### Concepts

1. [Raw sockets](https://man7.org/linux/man-pages/man7/raw.7.html)
1. [icmp](https://man7.org/linux/man-pages/man7/icmp.7.html)

### Functions

1. [getaddrinfo](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html):
dns/name host ip validation
1. [getnameinfo](https://man7.org/linux/man-pages/man3/getnameinfo.3.html):
reverse dns

## 🙏 Acknowledgments

Inspired by the inettools 2.0 ping utility.

Built with 💖 and a healthy ammount of coffee.
