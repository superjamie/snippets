/*exercise getaddrinfo(3) */

#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int ret = 0;
	struct sockaddr *addr = NULL;
	struct addrinfo *res = NULL;
	struct addrinfo *servinfo = NULL;

	char address_str[INET6_ADDRSTRLEN] = {0};
	char protocol_str[12] = {0};
	char socket_str[12] = {0};

	if (argc != 2) {
		printf("usage: %s hostname\n", argv[0]);
		return EXIT_FAILURE;
	}

	ret = getaddrinfo(argv[1], NULL, NULL, &servinfo);

	if (ret != 0) {
		printf("getaddrinfo: %s\n", gai_strerror(ret));
		return EXIT_FAILURE;
	}

	printf("Host %s\n", argv[1]);

	for (res = servinfo; res != NULL; res = res->ai_next) {
		switch(res->ai_family) {
			case AF_INET:
				addr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
				break;
			case AF_INET6:
				addr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
				break;
			default:
				break;
		}
		switch(res->ai_socktype) {
			case SOCK_STREAM:
				strcpy(socket_str, "SOCK_STREAM");
				break;
			case SOCK_DGRAM:
				strcpy(socket_str, "SOCK_DGRAM");
				break;
			case SOCK_RAW:
				strcpy(socket_str, "SOCK_RAW");
				break;
			default:
				sprintf(socket_str, "%d", res->ai_socktype);
				break;
		}
		switch(res->ai_protocol) {
			case IPPROTO_TCP:
				strcpy(protocol_str, "TCP");
				break;
			case IPPROTO_UDP:
				strcpy(protocol_str, "UDP");
				break;
			case IPPROTO_IP:
				strcpy(protocol_str, "IP");
				break;
			default:
				sprintf(protocol_str, "%d", res->ai_protocol);
				break;
		}
		inet_ntop(res->ai_family, addr, address_str, INET6_ADDRSTRLEN);
		printf(" %s (%s %s)\n", address_str, socket_str, protocol_str);
		memset(address_str, 0, sizeof(address_str));
		memset(socket_str, 0, sizeof(socket_str));
		memset(protocol_str, 0, sizeof(protocol_str));
	}

	freeaddrinfo(servinfo);

	return EXIT_SUCCESS;
}
