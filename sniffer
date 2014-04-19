#include<errno.h>		// errno and perror
#include<stdio.h>		// printf
#include<stdlib.h>		// malloc
#include<string.h>		// strlen
 
#include<netinet/if_ether.h>	// ETH_P_ALL
#include<net/ethernet.h>	// ethhdr
#include<netpacket/packet.h>	// sockaddr_ll
#include<net/if.h>		// IFNAMSIZ
#include<sys/socket.h>		// socket
#include<arpa/inet.h>		// htons
#include<sys/ioctl.h>		// ioctl
#include<unistd.h>		// close

//#include<sys/time.h>
//#include<sys/types.h>
//#include<netdb.h>
//#include<netinet/in.h>
//#include<netinet/ip_icmp.h>    //declarations for icmp header
//#include<netinet/udp.h>        //declarations for udp header
//#include<netinet/tcp.h>        //declarations for tcp header
//#include<netinet/ip.h>         //declarations for ip header

/* basic packet sniffer
 *
 * this takes one argument, an interface name, and starts packet sniffing on it
 * it prints out only src and dst mac, and the field where either the ethertype or vlan type goes
 * modified from http://www.binarytides.com/packet-sniffer-code-in-c-using-linux-sockets-bsd-part-2/
 */

// prototypes
 
void ProcessPacket(unsigned char* , int);
void print_ethernet_header(unsigned char* , int);

int iface_get_id(int, char*);
int iface_bind(int, int);

// functions
 
// from libpcap - gets the interface index of a given interface

int iface_get_id(int fd, char *device)
{
        struct ifreq    ifr;

        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));

        if (ioctl(fd, SIOCGIFINDEX, &ifr) != 0)
	{
                perror("SIOCGIFINDEX");
                return -1;
        }

        return ifr.ifr_ifindex;
}

// from libpcap - binds an existing AF_PACKET socket to an interface index

int iface_bind(int fd, int ifindex)
{
        struct sockaddr_ll      sll;

        memset(&sll, 0, sizeof(sll));
        sll.sll_family          = AF_PACKET;
        sll.sll_ifindex         = ifindex;
        sll.sll_protocol        = htons(ETH_P_ALL);

        if (bind(fd, (struct sockaddr *) &sll, sizeof(sll)) == -1) {
                if (errno == ENETDOWN) {
                        /*
                         * Return a "network down" indication, so that
                         * the application can report that rather than
                         * saying we had a mysterious failure and
                         * suggest that they report a problem to the
                         * libpcap developers.
                         */
                        perror("bind interface down");
			return ENETDOWN;
                } else {
                        perror("bind");
                        return -1;
                }
        }

        return 0;
}

// main entry point
 
int main(int argc, char *argv[])
{

    if(argc != 2)
    {
	printf("usage: %s [interface]\n", argv[0]);
	return 1;
    }

    char *interfacename = argv[1];

    int saddr_size, data_size;
    struct sockaddr saddr;
    
    unsigned char *buffer = (unsigned char *) malloc(65536); //Its Big!
     
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
     
    if(sock_raw < 0)
    {
        perror("socket");
        return 1;
    }

    // calls to interface binding funcs from libpcap

    int ifindex = 0;
    ifindex = iface_get_id(sock_raw, interfacename);
    if(ifindex < 0)
    {
	// perror("ifindex");
	return 1;
    }

    if( iface_bind(sock_raw, ifindex) != 0 )
    {
	perror("iface_bind");
	return 1;
    }

    printf("Starting sniffer on interface %s\n", argv[1]);

    while(1)
    {
        saddr_size = sizeof saddr;
        // receive a packet
        data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
        if(data_size <0 )
        {
            perror("recvfrom");
            return 1;
        }
        // now process the packet
        ProcessPacket(buffer , data_size);
    }
    close(sock_raw);
    printf("Finished.\n");
    return 0;
}
 
void ProcessPacket(unsigned char* buffer, int size)
{
    print_ethernet_header(buffer , size);
}
 
void print_ethernet_header(unsigned char* Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    // printf("Ethernet Header\n");
    printf(" src %.2X:%.2X:%.2X-%.2X:%.2X:%.2X --", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    printf(" dst %.2X:%.2X:%.2X-%.2X:%.2X:%.2X --", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    printf(" proto 0x%.4X \n",htons((__be16)eth->h_proto));
}
 
