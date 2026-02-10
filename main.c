#include "udp_lib/udp.h"
#include <getopt.h>
#include <stddef.h>
#include <string.h>

/* TODO mac address and gettors. */

int main(int argc, char ** argv) {
    int ret = 0;
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
        {"file", 1, NULL, 'f'}, \
        {NULL, 0, NULL, '\0'}, \
    };

    udp_pack_t pack = init_udp_pack();

    if (pack == NULL) {
        ret = -1;
        goto get_not_memory_udp_pack;
    }

    while (cmd) {
        cmd = getopt_long(argc, argv, "wi:s:p:o:n:f:", long_options, &option_index);

        switch (cmd) {
            case 'w':
                data = cmd;
                ret = set_input_data_udp_pack(pack);
                break;
            case 'i':
                ret = set_ip_destination_udp_pack(pack, optarg);
                break;
            case 's':
                ret = set_ip_source_udp_pack(pack, optarg);
                break;
            case 'p':
                ret = set_port_destination_udp_pack(pack, optarg);
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
    send_udp_pack(pack);
    destroy_udp_pack(pack);
    return 0;
error_in_action:
    destroy_udp_pack(pack);
get_not_memory_udp_pack:
    return ret;
}
