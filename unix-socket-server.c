#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>

#define SRV "srvsock"

/* UNIX socket server to test getpeername
 *
 * Usage: ./server
 *
 * Behaviour of getpeername depends on client, see unix(7).
 * If client sends string starting "close" then server exits gracefully.
 *
 * Based on http://osr507doc.sco.com/en/netguide/dusockT.code_samples.html
 */

int main(int argc, char *argv[]) {
	int sock, msgsock, rval, ret;
	struct sockaddr_un server, peer;
	char buf[1024];
	socklen_t len = sizeof(struct sockaddr_un);
	bool toclose = false;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	memset(&server, 0, sizeof(server));
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, SRV);

	ret = bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un));
	if (ret) {
		perror("bind");
		close(sock);
		exit(1);
	}

	printf("Socket has name %s\n", server.sun_path);

	listen(sock, 5);
	do {
		msgsock = accept(sock, 0, 0);
		if (-1 == msgsock) {
			perror("accept");
		} else do {
			bzero(buf, sizeof(buf));
			rval = read(msgsock, buf, 1024);
			if (rval > 0) {
				memset(&peer, 0, sizeof(peer));
				ret = getpeername(msgsock, (struct sockaddr *) &peer, &len);
				if (ret) {
					perror("getpeername");
				} else {
					printf("peer af   = %d\n", peer.sun_family);
					printf("peer plen = %ld\n", sizeof(peer.sun_path));
					printf("peer path = %s\n", peer.sun_path);
				}
				printf("Received: %s\n", buf);
			}
			if (rval < 0) {
				perror("read");
			} else if (!(strncmp("close", buf, 5))) {
				toclose = true;
			} else if (rval == 0) {
				printf("Ending connection\n");
			}
		} while (rval > 0);
		close(msgsock);
	} while (toclose == false);

	printf("Goodbye\n");
	close(sock);
	unlink(SRV);
	return 0;
}
