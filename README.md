# ft_ping

Recreating the inettools-2.0 ping in C

## Steps

1. Resolve FQDN before sending the packet:
use [getaddrinfo](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html) to
resolve the inputed domain name.
1. Use sendto to send messages
1. read the ip packet header into an iphdr/icmp struct
(They exist, look at <netinet/ip.h> and <netinet/ip_icmp.h>)

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
