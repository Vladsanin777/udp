#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>

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
    char * m_interface;
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

void set_port_source_udp_pack(udp_pack pack, const char * const port) {
    pack->m_head.m_port_source = htons(atoi(port));
}

void set_port_destination_udp_pack(udp_pack pack, const char * const port) {
    pack->m_head.m_port_destination = htons(atoi(port));
}

void set_ip_source_udp_pack(udp_pack pack, const char * const ip) {
    pack->m_iphdr.saddr = inet_addr(ip);
}

void set_ip_destination_udp_pack(udp_pack pack, const char * const ip) {
    pack->m_iphdr.daddr = inet_addr(ip);
}

void set_size_udp_pack(udp_pack pack, uint16_t size) {
    printf("new size udp pack: %d\n", size);
    pack->m_head.m_length = htons(HEAD_UDP + size);
    pack->m_iphdr.tot_len = htons(HEAD_UDP_IP + size);
}

uint16_t get_size_udp_pack(udp_pack pack) {
    return ntohs(pack->m_head.m_length) - HEAD_UDP;
}

void add_date_udp_pack(udp_pack pack, void * data, uint16_t size) {
    uint16_t old_size = get_size_udp_pack(pack);
    uint16_t new_raw_size = old_size + size;
    uint16_t new_size = MIN(new_raw_size, MAX_SIZE_DATA);
    uint16_t overflow_size = new_raw_size - new_size;
    printf("old size udp pack: %d\n", old_size);
    if (new_size == MAX_SIZE_DATA)
        size -= overflow_size;
    memcpy(pack->m_data + old_size, data, size);
    set_size_udp_pack(pack, new_size);
}

void set_data_udp_pack(udp_pack pack, void * data, uint16_t size) {
    size = MIN(size, MAX_SIZE_DATA);
    memcpy(pack->m_data, data, size);
    set_size_udp_pack(pack, size);
}

int add_symbol_udp_pack(udp_pack pack, uint8_t symbol) {
    int ret = 0;
    uint16_t size = get_size_udp_pack(pack);
    if (size == MAX_SIZE_DATA) {
        ret = -ENOMEM;
        goto error_overflow_max_size;
    }
    *((uint8_t *)pack->m_data + size) = symbol;
    set_size_udp_pack(pack, size + 1);
    return ret;
error_overflow_max_size:
    return ret;
}

int set_input_data_udp_pack(udp_pack pack) {
    int ret = 0;
    char symbol = '\0';
    while ((symbol = getchar()) != EOF) {
        ret = add_symbol_udp_pack(pack, symbol);
        if (ret)
            goto error_overflow_max_size;
    }
    return ret;
error_overflow_max_size:
    return ret;
}


int set_file_data_udp_pack(udp_pack pack, const char * const file_name) {
    int fd = 0;
    int ret = 0;
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

    ret = read(fd, pack->m_data, size);


    if (ret) {
        perror("ERROR: read not for set file data udp pack");
        goto error_read_not_for_set_file_data_udp_pack;
    }

    set_size_udp_pack(pack, size);

    close(fd);

    return ret;
error_read_not_for_set_file_data_udp_pack:
error_request_stat_for_set_data_udp_pack:
    close(fd);
get_not_fd_set_file_data:
    return ret;
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

void set_interface_udp_pack( \
        udp_pack pack, const char * const interface) {
    if (pack->m_interface)
        free(pack->m_interface);
    pack->m_interface = strdup(interface);
}

int send_udp_pack(udp_pack pack) {
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
    if (pack->m_interface)
        free(pack->m_interface);
    free(pack);
}

int main(int argc, char ** argv) {
    int data = '\0';
    int cmd = true;
    int option_index = 0;

    static struct option long_options[] = { \
        {"stdio", no_argument, NULL, 'w'}, \
        {"ip-destination", 1, NULL, 'i'}, \
        {"ip-source", 1, NULL, 's'}, \
        {"port-destanition", 1, NULL, 'p'}, \
        {"port-source", 1, NULL, 'o'}, \
        {"interface", 1, NULL, 'n'}, \
        {NULL, 0, NULL, '\0'}, \
    };

    udp_pack pack = init_udp_pack();

    while (cmd) {
        cmd = getopt_long(argc, argv, "wi:s:p:o:n:", long_options, &option_index);

        switch (cmd) {
            case 'w':
                data = cmd;
                set_input_data_udp_pack(pack);
                break;
            case 'i':
                set_ip_destination_udp_pack(pack, optarg);
                break;
            case 's':
                set_ip_source_udp_pack(pack, optarg);
                break;
            case 'p':
                set_port_destination_udp_pack(pack, optarg);
                break;
            case 'o':
                set_port_source_udp_pack(pack, optarg);
                break;
            case 'n':
                set_interface_udp_pack(pack, optarg);
                break;
            case 'f':
                data = cmd;
                set_file_data_udp_pack(pack, optarg);
                break;
            case '?':
                break;
            case -1:
                goto exit_parsing_comand;
            default:
                break;
        }
    }
exit_parsing_comand:
    if (data == '\0')
        for (; optind < argc; optind++)
            add_date_udp_pack(pack, argv[optind], strlen(argv[optind]));
    send_udp_pack(pack);
    destroy_udp_pack(pack);
    return 0;
}
