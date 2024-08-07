#define _POSIX_C_SOURCE 200112L

/**
 * TCP SYN Flood Listener
 *
 * Small program which listens on port 9001 with a small listen backlog to
 * help encourage SYN floods. Produce a SYN flood with hping3 or just a big
 * heap of netcat in the background.
 *
 * gcc -Wall -Wextra -Wpedantic -std=c99 -no-pie synflood-server.c -o synflood-server */

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void print_usage_and_exit(char *execname)
{
	printf("Usage: %s -4|-6 HOST\n", execname);
	printf("SYN flood listener.\n");
	printf("  -4  Use IPv4\n");
	printf("  -6  Use IPv6\n");
	printf("  HOST  Listen hostname\n");
	printf("    0.0.0.0   - IPv4 any\n");
	printf("    127.0.0.1 - IPv4 addr\n");
	printf("    ::        - IPv6 any\n");
	printf("    ::1       - IPv6 addr\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		print_usage_and_exit(argv[0]);

	bool ipv4_flag = false;
	bool ipv6_flag = false;

	int c = 0;
	while ((c = getopt (argc, argv, "46")) != -1)
		switch(c) {
		case '4':
			ipv4_flag = true;
			break;
		case '6':
			ipv6_flag = true;
			break;
		case '?':
		default:
			print_usage_and_exit(argv[0]);
			break;
		}

	if (
	    ((!ipv4_flag) && (!ipv6_flag)) ||
	    ((ipv4_flag) && (ipv6_flag))
	   ) {
		printf("ERROR: Specify either -4 or -6\n");
		print_usage_and_exit(argv[0]);
	}

	if (!argv[optind]) {
		printf("ERROR: No hostname provided\n");
		print_usage_and_exit(argv[0]);
	}

	int sockfd = 0;
	int connfd = 0;
	int res = 0;
	int my_domain = 0;

	if (ipv4_flag)
		my_domain = AF_INET;
	else
		my_domain = AF_INET6;

	sockfd = socket(my_domain, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	struct addrinfo hints = { 0 };
	struct addrinfo *result = NULL;

	hints.ai_family = my_domain;
	hints.ai_socktype = SOCK_STREAM;

	res = getaddrinfo(argv[optind], "9001", &hints, &result);
	if (res != 0) {
		printf("ERROR: getaddrinfo: %s\n", gai_strerror(res));
		exit(EXIT_FAILURE);
	}

	res = bind(sockfd, result->ai_addr, result->ai_addrlen);
	if (res == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	const int enable = 1;
	res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	if (res == -1) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	res = listen(sockfd, 1);
	if (res == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	} else {
		printf("listening...\n");
	}

	char buffer[80];

	while (true) {
		connfd = accept(sockfd, NULL, NULL);
		if (connfd == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		} else {
			printf(".");
			fflush(stdout);
		}

		(void) recv(connfd, buffer, sizeof(buffer), 0);

		res = close(connfd);
		if (res == -1) {
			perror("close");
			exit(EXIT_FAILURE);
		}
	}

	close(sockfd);
	return 0;
}
