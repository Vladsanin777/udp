/**
 * @file udp_lib/udp.c
 * @author Vladsanin777
 * @brief Code file for work udp package
 */

#include "udp_lib/udp.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>

#include <net/if.h>

#include <linux/if_packet.h>

#include <net/ethernet.h>

#include <arpa/inet.h>

#include <sys/socket.h>

#include <netinet/ip.h>

#define PACKED __attribute__((packed))

/** 
 * @ingroup UdpPack
 * @brief Struct is header UDP pack
 * @note This struct is private. Not used outside udp_lib/udp.c
 */
struct udp_head {
    uint16_t m_port_source; /**< Port source */
    uint16_t m_port_destantion; /**< Port destantion */
    uint16_t m_length; /**< Length udp pack */
    uint16_t m_checksum; /**< Calculated software checksum */
} PACKED;

#define HEAD_ETH sizeof(struct ethhdr)

#define HEAD_UDP sizeof(struct udp_head)

#define HEAD_IP sizeof(struct iphdr)

#define HEAD_UDP_IP (HEAD_IP + HEAD_UDP)

#define MAX_SIZE_DATA (0xFFFF - HEAD_UDP_IP)

#define NULL_CHECKSUM 0x0000

#define MIN(left, rigth) (((left) < ((typeof(left))rigth)) ? (left) : ((typeof(left))rigth))

/**
 * @ingroup UdpPack
 * @brief Struct is UDP package.
 * @note This struct is private. Not used outside udp_lib/udp.c
 */
struct udp_pack {
    char m_interface[IFNAMSIZ]; /**< Name ethrnet interface. */
    struct ethhdr m_ethhdr; /**< Ethernet header start UDP package. */
    struct iphdr m_iphdr; /**< IP header */
    struct udp_head m_head; /**< UDP header */
    uint8_t  m_data[MAX_SIZE_DATA]; /**< Data in UDP package. */
} PACKED;

udp_pack_t init_udp_pack(void) {
    udp_pack_t pack = calloc(1, sizeof(*pack));
    if (pack == NULL)
        goto get_not_memory;
    memset(pack->m_interface, 0x00, IFNAMSIZ);
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
    pack->m_head.m_port_destantion = htons(0x0000);
    pack->m_head.m_length = htons(HEAD_UDP);
    pack->m_head.m_checksum = htons(NULL_CHECKSUM);
    return pack;
get_not_memory:
    return NULL;
}

/**
 * @ingroup UdpPack
 * @brief Function support for parsing port.
 * @param[in] port Port in string format.
 * @return Port for big indian architect.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static uint16_t inet_port(const char * const port) {
    ssize_t ret = 0;
    uint16_t hport = 0;
    ssize_t decimal = atoi(port);
    
    if (decimal == 0) {
        ret = -1;
        goto error_in_atoi;
    }

    hport = htons(decimal);

    return hport;
error_in_atoi:
    return ret;
}

ssize_t set_port_source_udp_pack(udp_pack_t pack, const char * const port) {
    ssize_t ret = 0;
    uint16_t hport = inet_port(port);
    if (hport == (uint16_t)-1) {
        ret = -1;
        goto error_in_inet_port;
    }
    pack->m_head.m_port_source = hport;

    return ret;
error_in_inet_port:
    return ret;
}

ssize_t set_port_destantion_udp_pack(udp_pack_t pack, const char * const port) {
    ssize_t ret = 0;
    uint16_t hport = inet_port(port);
    if (hport == (uint16_t)-1) {
        ret = -1;
        goto error_in_inet_port;
    }
    pack->m_head.m_port_destantion = hport;

    return ret;
error_in_inet_port:
    return ret;
}

ssize_t set_ip_address_source_udp_pack(udp_pack_t pack, const char * const ip) {
    ssize_t ret = 0;

    pack->m_iphdr.saddr = inet_addr(ip);
    if (pack->m_iphdr.saddr == (in_addr_t)-1) {
        ret = -1;
        goto error_in_inet_addr;
    }
    return ret;
error_in_inet_addr:
    return ret;
}

ssize_t set_ip_address_destantion_udp_pack(udp_pack_t pack, const char * const ip) {
    ssize_t ret = 0;
    pack->m_iphdr.daddr = inet_addr(ip);
    if (pack->m_iphdr.daddr == (in_addr_t)-1) {
        ret = -1;
        goto error_in_inet_addr;
    }
    return ret;
error_in_inet_addr:
    return ret;
}

/**
 * @ingroup UdpPack
 * @brief Function raw write size UDP package.
 * @param[in,out] pack UDP package for work.
 * @param[in] size New size UDP package.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static void set_size_udp_pack(udp_pack_t pack, \
        const uint16_t size) {
    pack->m_head.m_length = htons(HEAD_UDP + size);
    pack->m_iphdr.tot_len = htons(HEAD_UDP_IP + size);
}

uint16_t get_size_data_udp_pack(udp_pack_t pack) {
    return ntohs(pack->m_head.m_length) - HEAD_UDP;
}


ssize_t add_date_udp_pack(udp_pack_t pack, void * data, \
        const uint16_t size) {
    int ret = 0;
    void * ptr = NULL;
    uint16_t size_copy = size;
    uint16_t old_size = get_size_data_udp_pack(pack);
    uint16_t new_raw_size = old_size + size;
    uint16_t new_size = MIN(new_raw_size, MAX_SIZE_DATA);
    uint16_t overflow_size = new_raw_size - new_size;
    if (new_size == MAX_SIZE_DATA)
        size_copy -= overflow_size;
    ptr = memcpy(pack->m_data + old_size, data, size_copy);
    if (ptr == NULL) {
        ret = -1;
        goto add_not_data_udp_pack;
    }
    set_size_udp_pack(pack, new_size);
    return ret;
add_not_data_udp_pack:
    return ret;
}

ssize_t set_data_udp_pack(udp_pack_t pack, void * data, uint16_t size) {
    ssize_t ret = 0;
    void * ptr = NULL;
    size = MIN(size, MAX_SIZE_DATA);
    ptr = memcpy(pack->m_data, data, size);
    if (ptr == NULL) {
        ret = -1;
        goto set_not_data_udp_pack;
    }
    set_size_udp_pack(pack, size);
    return ret;
set_not_data_udp_pack:
    return ret;
}

ssize_t add_byte_udp_pack(udp_pack_t pack, uint8_t byte) {
    ssize_t ret = 0;
    uint16_t size = get_size_data_udp_pack(pack);
    if (size == MAX_SIZE_DATA) {
        ret = -ENOMEM;
        goto error_overflow_max_size;
    }
    *((uint8_t *)pack->m_data + size) = byte;
    set_size_udp_pack(pack, size + 1);
    return ret;
error_overflow_max_size:
    return ret;
}

ssize_t set_input_data_udp_pack(udp_pack_t pack) {
    ssize_t ret = 0;
    ssize_t symbol = '\0';
    while ((symbol = getchar()) != EOF) {
        ret = add_byte_udp_pack(pack, symbol);
        if (ret)
            goto error_overflow_max_size;
    }
    ret = add_byte_udp_pack(pack, '\0');
    if (ret)
        goto error_overflow_max_size;
    return ret;
error_overflow_max_size:
    return ret;
}


ssize_t set_file_data_udp_pack(udp_pack_t pack, \
        const char * const file_name) {
    ssize_t fd = 0;
    ssize_t ret = 0;
    uint16_t size = 0;
    struct stat st;
    fd = open(file_name, O_RDONLY);
    if (fd < 0) {
        perror("ERROR: get not fd for set file data udp pack");
        ret = fd;
        goto get_not_fd_set_file_data;
    }
    ret = fstat(fd, &st);

    if (ret) {
        perror("ERROR: error request stat for set file data udp pack");
        goto error_request_stat_for_set_data_udp_pack;
    }

    size = MIN(st.st_size, MAX_SIZE_DATA);

    {
        ssize_t new_size = read(fd, pack->m_data, size);


        if (new_size == -1) {
            ret = new_size;
            perror("ERROR: read not for set file data udp pack");
            goto error_read_not_for_set_file_data_udp_pack;
        }

        set_size_udp_pack(pack, new_size);
    }

    close(fd);

    return ret;
error_read_not_for_set_file_data_udp_pack:
error_request_stat_for_set_data_udp_pack:
    close(fd);
get_not_fd_set_file_data:
    return ret;
}

/**
 * @ingroup UdpPack
 * @brief Function calculate sum in big endian.
 * @param[in] ptr Buffer for calculating sum in big endian.
 * @param[in] nbytes Size buffer for calculating sum.
 * @return Sum buffer in big endian.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static uint32_t sum_compute(void *ptr, \
        uint16_t nbytes) {
    uint32_t sum = htonl(0x00000000);

    for (; nbytes > 1; nbytes -= 2, ptr+=2)
        sum += htons(*(uint16_t *)ptr);

    if (nbytes == 1)
        sum += htons(*(uint8_t *)ptr);

    return sum;
}

/**
 * @ingroup UdpPack
 * @brief Function calculating from sum big endian to checksum big endian.
 * @param[in] sum Sum in big endian.
 * @return Checksum in big endian.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static uint16_t checksum_compute(uint32_t sum) {
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    if (sum == 0xFFFF)
        return sum;

    return htons(~sum);
}

/**
 * @ingroup UdpPack
 * @brief Struct pseudo_header for calculate checksum for UDP package.
 * @note This struct is private. Not used outside udp_lib/udp.c
 */
struct pseudo_header {
    uint32_t source_address; /**< Source ip addresses */
    uint32_t dest_address; /**< Destination ip addresses */
    uint8_t placeholder; /**< Placeholder is 0x00. */
    uint8_t protocol; /**< Constant protocol. */
    uint16_t udp_length; /**< Length UDP package. */
} PACKED;

#define HEAD_PSEUDO sizeof(struct pseudo_header)

/**
 * @ingroup UdpPack
 * @brief Function calculate checksum for ip header and UDP package.
 * @param[in,out] pack UDP package for work.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static void calculate_checksum_udp_pack(udp_pack_t pack) {
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

ssize_t set_interface_udp_pack( \
        udp_pack_t pack, const char * const interface) {
    ssize_t ret = 0;

    strncpy(pack->m_interface, interface, IFNAMSIZ);

    return ret;
}

/**
 * @ingroup UdpPack
 * @brief Function raw get pointer on UDP package.
 * @param[in,out] pack UDP package for work.
 * @return Getting UDP package.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static void * get_pack_udp_pack(udp_pack_t pack) {
    return &pack->m_ethhdr;
}

ssize_t send_udp_pack(udp_pack_t pack) {
    struct ifreq ifr;
    struct sockaddr_ll sockaddr_ll = {0};
    ssize_t fd_sock = 0;
    ssize_t ret = 0;

    calculate_checksum_udp_pack(pack);

    fd_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd_sock < 0) {
        ret = fd_sock;
        perror("ERROR: get not fd sock, please lauhce with root");
        goto give_not_fd_socket;
    }

    strncpy(ifr.ifr_name, pack->m_interface, IFNAMSIZ);
    ret = ioctl(fd_sock, SIOCGIFINDEX, &ifr);

    if (ret) {
        perror("ERROR: get not siocgifindex");
        goto give_not_siocgifindex;
    }

    sockaddr_ll.sll_family = AF_PACKET;
    sockaddr_ll.sll_ifindex = ifr.ifr_ifindex;
    sockaddr_ll.sll_halen = ETH_ALEN;
    memset(sockaddr_ll.sll_addr, 0xff, ETH_ALEN);

    sendto(fd_sock, get_pack_udp_pack(pack), ETH_HLEN + ntohs(pack->m_iphdr.tot_len), \
           0, (struct sockaddr *)&sockaddr_ll, sizeof(sockaddr_ll));

    puts("\nPacked sended!!!");
    close(fd_sock);
    return ret;
give_not_fd_socket:
give_not_siocgifindex:
    return ret;
}

/**
 * @ingroup UdpPack
 * @brief Function return mac address in big endian.
 * @param[in,out] pack UDP package for work.
 * @return Getting buffer mac address.
 * @note This function is private. Not used outside udp_lib/udp.c
 */
static void * inet_mac(const char * const mac_address) {
    ssize_t ret = 0;
    uint8_t * mac_buffer = NULL;
    mac_buffer = (uint8_t *)calloc((size_t)sizeof(*mac_buffer), (size_t)6);
    if (mac_buffer == NULL)
        goto get_not_buffer;
    ret = sscanf(mac_address, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", \
            mac_buffer, mac_buffer + 1, mac_buffer + 2, \
            mac_buffer + 3, mac_buffer + 4, mac_buffer + 5);
    if (ret != 6)
        goto parce_not_mac_address;

    return (void *)mac_buffer;
parce_not_mac_address:
    free(mac_buffer);
    mac_buffer = NULL;
get_not_buffer:
    return (void *)mac_buffer;
}

ssize_t set_mac_address_source_udp_pack( \
        udp_pack_t pack, const char * const mac_address) {
    ssize_t ret = 0;
    void * ptr = NULL;
    void * mac = NULL;
    ptr = inet_mac(mac_address);
    if (ptr == NULL) {
        ret = -1;
        goto get_not_mac_address;
    }
    mac = ptr;
    ptr = memcpy(&pack->m_ethhdr.h_source, mac, ETH_ALEN);
    if (ptr == NULL) {
        ret = -1;
        goto write_not_mac_address;
    }
    free(mac);
    return ret;
write_not_mac_address:
    free(mac);
get_not_mac_address:
    return ret;
}

ssize_t set_mac_address_destantion_udp_pack( \
        udp_pack_t pack, const char * const mac_address) {
    ssize_t ret = 0;
    void * ptr = NULL;
    void * mac = NULL;
    ptr = inet_mac(mac_address);
    if (ptr == NULL) {
        ret = -1;
        goto get_not_mac_address;
    }
    mac = ptr;
    ptr = memcpy(&pack->m_ethhdr.h_dest, mac, ETH_ALEN);
    if (ptr == NULL) {
        ret = -1;
        goto write_not_mac_address;
    }
    free(mac);
    return ret;
write_not_mac_address:
    free(mac);
get_not_mac_address:
    return ret;
}

char * get_ip_address_source_udp_pack(udp_pack_t pack) {
    char * buffer = NULL;
    struct in_addr addr = {.s_addr = pack->m_iphdr.saddr};

    buffer = calloc(INET_ADDRSTRLEN, 1);

    if (buffer == NULL)
        goto get_not_buffer;

    if (inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN) == NULL)
        goto convert_not_ip_address;

    return buffer;
convert_not_ip_address:
    free(buffer);
get_not_buffer:
    return NULL;
}

char * get_ip_address_destantion_udp_pack(udp_pack_t pack) {
    char * buffer = NULL;
    struct in_addr addr = {.s_addr = pack->m_iphdr.daddr};

    buffer = calloc(INET_ADDRSTRLEN, 1);

    if (buffer == NULL)
        goto get_not_buffer;

    if (inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN) == NULL)
        goto convert_not_ip_address;

    return buffer;
convert_not_ip_address:
    free(buffer);
get_not_buffer:
    return NULL;
}

char * get_interface_udp_pack(udp_pack_t pack) {
    return strdup(pack->m_interface);
}

char * get_mac_address_source_udp_pack(udp_pack_t pack) {
    ssize_t ret = 0;
    char * mac_address = NULL;
    uint8_t * mac_buffer = pack->m_ethhdr.h_source;
    ret = asprintf(&mac_address, \
            "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", \
            *mac_buffer, mac_buffer[1], mac_buffer[2], \
            mac_buffer[3], mac_buffer[4], mac_buffer[5]);
    if (mac_address == NULL)
        goto get_not_buffer;

    if (ret != 17)
        goto get_not_mac_address;

    return mac_address;
get_not_mac_address:
    free(mac_address);
get_not_buffer:
    return NULL;
}

char * get_mac_address_destantion_udp_pack(udp_pack_t pack) {
    ssize_t ret = 0;
    char * mac_address = NULL;
    uint8_t * mac_buffer = pack->m_ethhdr.h_dest;
    ret = asprintf(&mac_address, \
            "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", \
            *mac_buffer, mac_buffer[1], mac_buffer[2], \
            mac_buffer[3], mac_buffer[4], mac_buffer[5]);
    if (mac_address == NULL)
        goto get_not_buffer;

    if (ret != 17)
        goto get_not_mac_address;

    return mac_address;
get_not_mac_address:
    free(mac_address);
get_not_buffer:
    return NULL;
}

char * get_port_destantion_udp_pack(udp_pack_t pack) {
    int ret = 0;
    char * port = NULL;
    ret = asprintf(&port, "%hd", \
            ntohs(pack->m_head.m_port_destantion));
    if (port == NULL)
        goto get_not_buffer;
    if (ret == 0)
        goto get_not_port;
    return port;
get_not_port:
    free(port);
get_not_buffer:
    return NULL;
}

char * get_port_source_udp_pack(udp_pack_t pack) {
    int ret = 0;
    char * port = NULL;
    ret = asprintf(&port, "%hd", \
            ntohs(pack->m_head.m_port_source));
    if (port == NULL)
        goto get_not_buffer;
    if (ret == 0)
        goto get_not_port;
    return port;
get_not_port:
    free(port);
get_not_buffer:
    return NULL;
}

ssize_t print_udp_pack(udp_pack_t pack) {
    int ret = 0;
    char * buffer = NULL;

    buffer = get_interface_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_interface;
    }

    puts("interface:");

    puts(buffer);

    free(buffer);

    buffer = get_mac_address_source_udp_pack(pack);
    
    if (buffer == NULL) {
        ret = -1;
        goto get_not_mac_address_source;
    }

    puts("mac address source:");

    puts(buffer);

    free(buffer);

    buffer = get_mac_address_destantion_udp_pack(pack);
    
    if (buffer == NULL) {
        ret = -1;
        goto get_not_mac_address_destantion;
    }

    puts("mac address destantion:");

    puts(buffer);

    free(buffer);

    buffer = get_ip_address_source_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_ip_address_source;
    }

    puts("ip address source:");

    puts(buffer);

    free(buffer);

    buffer = get_ip_address_destantion_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_ip_address_destantion;
    }

    puts("ip address destantion:");

    puts(buffer);

    free(buffer);

    buffer = get_port_source_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_port_source;
    }

    puts("port source:");

    puts(buffer);

    free(buffer);

    buffer = get_port_destantion_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_port_destantion;
    }

    puts("port destantion:");

    puts(buffer);

    free(buffer);

    buffer = get_data_udp_pack(pack);

    if (buffer == NULL) {
        ret = -1;
        goto get_not_data;
    }

    puts("data:");

    write(STDOUT_FILENO, buffer, get_size_data_udp_pack(pack));

    free(buffer);

    return ret;

get_not_data:
get_not_port_destantion:
get_not_port_source:
get_not_ip_address_destantion:
get_not_ip_address_source:
get_not_mac_address_destantion:
get_not_mac_address_source:
get_not_interface:
    
    return ret;
}

char * get_data_udp_pack(udp_pack_t pack) {
    char * buffer = NULL;
    void * tmp = NULL;
    uint16_t size_data = get_size_data_udp_pack(pack);
    buffer = calloc(size_data, 1);
    if (buffer == NULL)
        goto get_not_buffer;

    tmp = memcpy(buffer, pack->m_data, size_data);

    if (tmp == NULL)
        goto copy_not_data;
    return buffer;
copy_not_data:
    free(buffer);
get_not_buffer:
    return NULL;
}

void destroy_udp_pack(udp_pack_t pack) {
    free(pack);
}
