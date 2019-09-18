#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>

#define SRV "srvsock"
#define CLI "clisock"
#define DATA "Hello"

/* UNIX socket client to test getpeername
 *
 * Usage: ./client [bind|close]
 *
 * If "bind" is given as argument, client will bind and get a pathname socket.
 * If "bind" is not given, client will not bind and get an unnamed socket.
 * See unix(7) for definition and behaviour of pathname/unnamed sockets.
 *
 * If "close" is given as argument, server is instructed to close.
 *
 * Based on http://osr507doc.sco.com/en/netguide/dusockT.code_samples.html
 */

int main(int argc, char *argv[]) {
	int sock, ret;
	struct sockaddr_un server, client, peer;
	socklen_t len = sizeof(struct sockaddr_un);
	bool tobind = false;
	bool toclose = false;

	if (argc == 2) {
		if (!(strncmp("bind", argv[1], 4))) {
			tobind = true;
		}
		if (!(strncmp("close", argv[1], 5))) {
			toclose = true;
		}
	}

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	memset(&client, 0, sizeof(client));
	client.sun_family = AF_UNIX;
	strcpy(client.sun_path, CLI);

	if (tobind) {
		ret = bind(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un));
		if (ret) {
			perror("bind");
			close(sock);
			unlink(CLI);
			exit(1);
		}
	}

	memset(&server, 0, sizeof(server));
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, SRV);

	ret = connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un));
	if (ret) {
		perror("connect");
		close(sock);
		unlink(CLI);
		exit(1);
	}

	memset(&peer, 0, sizeof(peer));
	ret = getpeername(sock, (struct sockaddr *) &peer, &len);
	if (ret) {
		perror("getpeername");
	} else {
		printf("peer af   = %d\n", peer.sun_family);
		printf("peer plen = %ld\n", sizeof(peer.sun_path));
		printf("peer path = %s\n", peer.sun_path);
	}

	if (toclose) {
		ret = write(sock, "close", sizeof("close"));
	} else {
		ret = write(sock, "hello", sizeof("hello"));
	}
	if (ret < 0) {
		perror("write");
	}

	close(sock);
	unlink(CLI);
	return 0;
}
