#include <stdint.h>

#define MAX_SIZE_DATA 0xFFEB

#define MIN_LENGTH_HEADS_UDP_FOR_NAT 0x0014

#define UDP_PROTOCOL 0x0011

#define NULL_CHECKSUM 0x0000

#define MIN(left, rigth) (((left) < (rigth)) ? (left) : (rigth))

struct udp_pack {
    uint32_t m_ip_source;
    uint32_t m_ip_target;
    uint16_t m_protocol;
    uint16_t m_length_first;
    uint16_t m_port_source;
    uint16_t m_port_target;
    uint16_t m_length_second;
    uint16_t m_checksum;
    uint8_t  m_data[MAX_SIZE_DATA + 1];
};

typedef struct udp_pack * udp_pack;

udp_pack init_udp_pack(void) {
    udp_pack pack = calloc(1, sizeof(*udp_pack));
    pack->m_ip_source = 0x00000000;
    pack->m_ip_target = 0x00000000;
    pack->m_protocol = UDP_PROTOCOL;
    pack->m_length_first = MIN_LENGTH_HEADS_UDP_FOR_NAT;
    pack->m_port_source = 0x0000;
    pack->m_port_target = 0x0000;
    pack->m_length_second = MIN_LENGTH_HEADS_UDP_FOR_NAT;
    pack->m_checksum = NULL_CHECKSUM;
}

void set_ip_source_udp_pack(udp_pack pack, uint32_t ip) {
    pack->m_ip_source = ip;
}

void set_ip_target_udp_pack(udp_pack pack, uint32_t ip) {
    pack->m_ip_target = ip;
}

void set_port_source_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_port_source = port;
}

void set_port_target_udp_pack(udp_pack pack, uint16_t port) {
    pack->m_port_target = port;
}

void set_data(udp_pack pack, void * data, size_t size) {
    uint16_t size = MIN(size, MAX_SIZE_DATA);
    pack->m_length_first = pack->m_length_second = \
                        MIN_LENGTH_HEADS_UDP_FOR_NAT + size;
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

void destroy_udp_pack(udp_pack pack) {
    free(pack);
}
