#include "udp_lib/udp.h"
#include <getopt.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Entry point for the UDP packet crafting and transmission utility.
 * 
 * This program parses command-line arguments to build a custom UDP packet. 
 * It allows configuration of MAC addresses, IP addresses, ports, and payload data.
 * 
 * @details
 * **Command-line Options:**
 * - `-w`, `--stdio`                  Read payload data from standard input.
 * - `-e`, `--print`                  Print the packet structure to the console before sending.
 * - `-i`, `--ip-address-destination` Set the destination IPv4 address.
 * - `-s`, `--ip-address-source`      Set the source IPv4 address.
 * - `-p`, `--port-destanition`       Set the destination UDP port.
 * - `-o`, `--port-source`            Set the source UDP port.
 * - `-n`, `--interface`              Specify the network interface (e.g., eth0).
 * - `-f`, `--file`                   Read payload data from a specified file.
 * - `-m`, `--mac-address-destantion` Set the destination MAC address.
 * - `-a`, `--mac-address-source`     Set the source MAC address.
 * 
 * **Payload Logic:**
 * 1. If `-w` or `-f` is provided, the data is pulled from those sources.
 * 2. If no source flag is provided, the program concatenates all remaining 
 *    positional arguments (argv) into a single space-separated string payload.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return 
 * - `0` on successful transmission.
 * - `-1` if memory allocation fails (`init_udp_pack`).
 * - Non-zero error code if packet configuration or sending fails.
 */
int main(int argc, char ** argv) {
    int ret = 0;
    int data = '\0';
    int cmd = true;
    bool is_print = false;
    int option_index = 0;

    static struct option long_options[] = { \
        {"stdio", no_argument, NULL, 'w'}, \
        {"print", no_argument, NULL, 'e'}, \
        {"ip-address-destination", 1, NULL, 'i'}, \
        {"ip-address-source", 1, NULL, 's'}, \
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
        cmd = getopt_long(argc, argv, "wei:s:p:o:n:f:m:a:", long_options, &option_index);

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
    if (data == '\0') {
        for (; optind < argc - 1; optind++) {
            ret = add_date_udp_pack(pack, argv[optind], strlen(argv[optind]));
            if (ret)
                goto error_in_action;

            ret = add_byte_udp_pack(pack, ' ');
            if (ret)
                goto error_in_action;
        }
        ret = add_date_udp_pack(pack, argv[optind], strlen(argv[optind]));
        if (ret)
            goto error_in_action;
        ret = add_byte_udp_pack(pack, '\0');
        if (ret)
            goto error_in_action;
    }
    if (is_print)
        ret = print_udp_pack(pack);
    if (ret)
        goto error_in_action;
    ret = send_udp_pack(pack);
    if (ret)
        goto send_not_udp_pack;
    destroy_udp_pack(pack);
    return ret;
send_not_udp_pack:
error_in_action:
    destroy_udp_pack(pack);
get_not_memory_udp_pack:
    return ret;
}
