#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <net/if.h>

#include <linux/if_packet.h>

#include <net/ethernet.h>

#include <arpa/inet.h>

#include <sys/socket.h>

#include <netinet/ip.h>

struct udp_head {
    uint16_t m_port_source;
    uint16_t m_port_destination;
    uint16_t m_length;
    uint16_t m_checksum;
};

#define HEAD_ETH sizeof(struct ethhdr)

#define HEAD_UDP sizeof(struct udp_head)

#define HEAD_IP sizeof(struct iphdr)

#define HEAD_UDP_IP (HEAD_IP + HEAD_UDP)

#define MAX_SIZE_DATA (0xFFFF - HEAD_UDP_IP)

#define NULL_CHECKSUM 0x0000

#define MIN(left, rigth) (((left) < (rigth)) ? (left) : (rigth))

struct __attribute__((packed)) udp_pack {
    struct ethhdr m_ethhdr;
    struct iphdr m_iphdr;
    struct udp_head m_head;
    uint8_t  m_data[MAX_SIZE_DATA];
};

typedef struct udp_pack * udp_pack;

udp_pack init_udp_pack(void) {
    udp_pack pack = calloc(1, sizeof(*pack));
    memset(pack->m_ethhdr.h_dest, 0xff, ETH_ALEN);
    memset(pack->m_ethhdr.h_source, 0x00, ETH_ALEN);
    pack->m_ethhdr.h_proto = htons(ETH_P_IP);

    pack->m_iphdr.version = 4;
    pack->m_iphdr.ihl = 5;
    pack->m_iphdr.protocol = IPPROTO_UDP;
    pack->m_iphdr.saddr = inet_addr("171.0.0.1");
    pack->m_iphdr.daddr = inet_addr("171.0.0.1");
    pack->m_iphdr.tot_len = htons(HEAD_UDP_IP);
    pack->m_iphdr.ttl = 64;

    pack->m_head.m_port_source = htons(0x0000);
    pack->m_head.m_port_destination = htons(0x0000);
    pack->m_head.m_length = htons(HEAD_UDP);
    pack->m_head.m_checksum = htons(NULL_CHECKSUM);
    return pack;
}

void set_port_source_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_head.m_port_source = htons(port);
}

void set_port_destination_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_head.m_port_destination = htons(port);
}

void set_ip_source_udp_pack(udp_pack pack, const char * const ip) {
    pack->m_iphdr.saddr = inet_addr(ip);
}

void set_ip_destination_udp_pack(udp_pack pack, const char * const ip) {
    pack->m_iphdr.daddr = inet_addr(ip);
}

void set_data_udp_pack(udp_pack pack, void * data, uint16_t size) {
    size = MIN(size, MAX_SIZE_DATA);
    pack->m_head.m_length = htons(HEAD_UDP + size);
    pack->m_iphdr.tot_len = htons(HEAD_UDP_IP + size);
    printf("%hd\n", ntohs(pack->m_head.m_length));
    memcpy(pack->m_data, data, size);
}

uint32_t sum_compute(void *ptr, int nbytes) {
    uint32_t sum = htons(0x00);

    for (; nbytes > 1; nbytes -= 2, ptr+=2)
        sum += htons(*(uint16_t *)ptr);

    if (nbytes == 1)
        sum += htons(*(uint8_t *)ptr);

    return sum;
}

uint16_t checksum_compute(uint32_t sum) {
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    if (sum == 0xFFFF)
        return sum;

    return htons(~sum);
}

struct pseudo_header {
    uint32_t source_address;
    uint32_t dest_address;
    uint8_t placeholder;
    uint8_t protocol;
    uint16_t udp_length;
};

#define HEAD_PSEUDO sizeof(struct pseudo_header)

void calculate_checksum_udp_pack(udp_pack pack) {
    struct pseudo_header psh;

    pack->m_iphdr.check = NULL_CHECKSUM;
    pack->m_iphdr.check = checksum_compute(sum_compute(&pack->m_iphdr, HEAD_IP));

    pack->m_head.m_checksum = NULL_CHECKSUM;

    psh.source_address = pack->m_iphdr.saddr;
    psh.dest_address = pack->m_iphdr.daddr;
    psh.placeholder = 0x00;
    psh.protocol = pack->m_iphdr.protocol;
    psh.udp_length = pack->m_head.m_length;
    pack->m_head.m_checksum = checksum_compute(sum_compute(&psh, HEAD_PSEUDO) + \
            sum_compute(&pack->m_head, ntohs(pack->m_head.m_length)));
}

int send_interface_udp_pack(udp_pack pack, const char * const interface) {
    struct ifreq ifr;
    struct sockaddr_ll sockaddr_ll = {0};
    int fd_sock = 0;
    int ret = 0;

    calculate_checksum_udp_pack(pack);

    fd_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd_sock < 0) {
        ret = fd_sock;
        perror("ERROR: get not fd sock, please lauhce with root");
        goto give_not_fd_socket;
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    ret = ioctl(fd_sock, SIOCGIFINDEX, &ifr);

    if (ret) {
        perror("ERROR: get not siocgifindex");
        goto give_not_siocgifindex;
    }

    sockaddr_ll.sll_family = AF_PACKET;
    sockaddr_ll.sll_ifindex = ifr.ifr_ifindex;
    sockaddr_ll.sll_halen = ETH_ALEN;
    memset(sockaddr_ll.sll_addr, 0xff, ETH_ALEN);

    sendto(fd_sock, pack, ETH_HLEN + ntohs(pack->m_iphdr.tot_len), \
           0, (struct sockaddr *)&sockaddr_ll, sizeof(sockaddr_ll));

    puts("Packed sended!!!");
    close(fd_sock);
    return ret;
give_not_fd_socket:
give_not_siocgifindex:
    return ret;
}

void destroy_udp_pack(udp_pack pack) {
    free(pack);
}

int main(int argc, char ** argv) {
    udp_pack pack = init_udp_pack();
    set_port_source_udp_pack(pack, 8001);
    set_port_destination_udp_pack(pack, 8003);
    set_ip_source_udp_pack(pack, "171.0.0.1");
    set_ip_destination_udp_pack(pack, "171.0.0.1");
    set_data_udp_pack(pack, "test", 5);
    send_interface_udp_pack(pack, argv[1]);
    destroy_udp_pack(pack);
    return 0;
}
