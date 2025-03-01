# ft_ping

Recreating the inettools-2.0 ping in C

## Steps

1. Resolve FQDN before sending the packet:
use [getaddrinfo](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html) to
resolve the inputed domain name.
1. Create an abstraction to interact with ip packets: read an ip packet into a struct.
1. Create an abstraction to deal with ICMP packets.
1. parse the flags into a struct.
