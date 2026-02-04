#include <stdint.h>

#include <net/if.h>

#include <linux/if_packet.h>

#include <net/ethernet.h>

#include <arpa/inet.h>

#include <sys/socket.h>

#define MAX_SIZE_DATA 0xFFF7

#define HEADS_UDP 0x0008

#define UDP_PROTOCOL 0x0011

#define NULL_CHECKSUM 0x0000

#define MIN(left, rigth) (((left) < (rigth)) ? (left) : (rigth))

struct __attribute__((packed)) udp_pack {
    uint16_t m_port_source;
    uint16_t m_port_target;
    uint16_t m_length;
    uint16_t m_checksum;
    uint8_t  m_data[MAX_SIZE_DATA + 1];
};

typedef struct __attribute__((packed)) udp_pack * udp_pack;

udp_pack init_udp_pack(void) {
    udp_pack pack = calloc(1, sizeof(*udp_pack));
    pack->m_port_source = 0x0000;
    pack->m_port_target = 0x0000;
    pack->m_length_second = HEADS_UDP;
    pack->m_checksum = NULL_CHECKSUM;
}

void set_port_source_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_port_source = port;
}

void set_port_target_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_port_target = port;
}

void set_data(udp_pack pack, void * data, size_t size) {
    uint16_t size = MIN(size, MAX_SIZE_DATA);
    pack->m_length = MIN_LENGTH_HEADS_UDP_FOR_NAT + size;
    memcpy(pack->m_data, data, size);
}

void calculate_checksum_udp_pack(udp_pack pack) {
    uint16_t length = pack->m_length_first;
    uint16_t checksum = NULL_CHECKSUM;
    uint16_t * ptr = (uint16_t *)pack;
    uint16_t * ptr_end = (uint8_t *)pack+length;
    pack->m_checksum = NULL_CHECKSUM;
    for (; ptr < ptr_end; ptr++)
        checksum += *ptr;
    pack->m_checksum = checksum;
}

int send_udp_pack_interface(udp_pack pack) {
    struct sockaddr_ll sockaddr = { 0 };
    int fd_sock = 0;
    int ret = 0;

    fd_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd_sock == -1) {
        ret = fd_sock;
        perror("ERROR: get fd sock, please lauhce with root");
        goto give_not_fd_socket;
    }

    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_ifindex = if_index;
    sockaddr.sll_halen = ETH_ALEN;
    sockaddr.sll_protocol = htons(ETH_P_ALL);

    memset(sockaddr.sll_addr, 0xff, ETH_ALEN);

    ret = sendto(fd_sock, pack, pack->m_length, 0, \
            (struct sockaddr *)&sockaddr, sizeof(sockaddr));

    if (ret) {
        perror("ERROR: send not UDP pack");
        goto send_not_udp_pack;
    }

    return ret;
give_not_fd_socket:
send_not_udp_pack:
    return ret;
}

void destroy_udp_pack(udp_pack pack) {
    free(pack);
}
