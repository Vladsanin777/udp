#include "udp_lib/udp.h"
#include <getopt.h>
#include <stddef.h>
#include <string.h>

/* TODO mac address and gettors. */

int main(int argc, char ** argv) {
    int ret = 0;
    int data = '\0';
    int cmd = true;
    bool is_print = false;
    int option_index = 0;

    static struct option long_options[] = { \
        {"stdio", no_argument, NULL, 'w'}, \
        {"print", no_argument, NULL, 'e'}, \
        {"ip-destination", 1, NULL, 'i'}, \
        {"ip-source", 1, NULL, 's'}, \
        {"port-destanition", 1, NULL, 'p'}, \
        {"port-source", 1, NULL, 'o'}, \
        {"interface", 1, NULL, 'n'}, \
        {"file", 1, NULL, 'f'}, \
        {"mac-address-destantion", 1, NULL, 'm'}, \
        {"mac-address-source", 1, NULL, 'a'}, \
        {NULL, 0, NULL, '\0'}, \
    };

    udp_pack_t pack = init_udp_pack();

    if (pack == NULL) {
        ret = -1;
        goto get_not_memory_udp_pack;
    }

    while (cmd) {
        cmd = getopt_long(argc, argv, "wi:s:p:o:n:f:m:a:", long_options, &option_index);

        switch (cmd) {
            case 'w':
                data = cmd;
                ret = set_input_data_udp_pack(pack);
                break;
            case 'e':
                is_print = true;
                break;
            case 'i':
                ret = set_ip_address_destantion_udp_pack(pack, optarg);
                break;
            case 's':
                ret = set_ip_address_source_udp_pack(pack, optarg);
                break;
            case 'p':
                ret = set_port_destantion_udp_pack(pack, optarg);
                break;
            case 'o':
                ret = set_port_source_udp_pack(pack, optarg);
                break;
            case 'n':
                ret = set_interface_udp_pack(pack, optarg);
                break;
            case 'f':
                data = cmd;
                ret = set_file_data_udp_pack(pack, optarg);
                break;
            case 'm':
                ret = set_mac_address_destantion_udp_pack(pack, optarg);
                break;
            case 'a':
                ret = set_mac_address_source_udp_pack(pack, optarg);
            case '?':
                break;
            case -1:
                goto exit_parsing_comand;
            default:
                break;
        }
        if (ret)
            goto error_in_action;
    }
exit_parsing_comand:
    if (data == '\0')
        for (; optind < argc; optind++)
            add_date_udp_pack(pack, argv[optind], strlen(argv[optind]));
    if (is_print)
        print_udp_pack(pack);
    if (send_udp_pack(pack) == -1)
        goto send_not_udp_pack;
    destroy_udp_pack(pack);
    return ret;
send_not_udp_pack:
error_in_action:
    destroy_udp_pack(pack);
get_not_memory_udp_pack:
    if (true) {
        get_ip_address_destantion_udp_pack(pack);
        get_ip_address_source_udp_pack(pack);
        get_port_destantion_udp_pack(pack);
        get_port_source_udp_pack(pack);
        get_interface_udp_pack(pack);
        get_mac_address_destantion_udp_pack(pack);
        get_mac_address_source_udp_pack(pack);
    }
    return ret;
}
