/**
 * @file udp_lib/udp.h
 * @author Vladsanin777
 * @brief Header file for work udp package.
 */

#include <stdint.h>
#include <stdlib.h>

/**
 * @defgroup UdpPack work for udp
 * @brief Group function for work udp package.
 * @{
 */

/**
 * @brief Private struct UDP package. (Hidden implementation)
 */
struct udp_pack;

/**
 * @brief UDP packet descriptor.
 *
 * The primary type for working with the library. All functions accept this pointer as an argument.
 */
typedef struct udp_pack * udp_pack_t;

/**
 * @brief Function for create object UDP package.
 * @note You must call @ref destroy_udp_pack after this.
 * @return pointer or null on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * // other code whit using udp_pack_t
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
udp_pack_t init_udp_pack(void);

/**
 * @brief Function for setting source port in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] port Source port to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_port_source_udp_pack(pack, "8001");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_port_source_udp_pack(udp_pack_t pack, const char * const port);

/**
 * @brief Function for setting destantion port in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] port Destination port to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_port_destantion_udp_pack(pack, "8003");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_port_destantion_udp_pack(udp_pack_t pack, const char * const port);

/**
 * @brief Function for setting source ip address in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] ip Source ip to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_ip_address_source_udp_pack(pack, "127.0.0.1");
 * if (ret == -1)
 *     goto set_not_ip_address;
 * // other code whit udp_pack_t
 * set_not_ip_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_ip_address_source_udp_pack( \
        udp_pack_t pack, const char * const ip);

/**
 * @brief Function for setting destantion ip address in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] ip Destination ip address to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_ip_address_destantion_udp_pack(pack, "127.0.0.1");
 * if (ret == -1)
 *     goto setting_not_ip_address;
 * // other code whit udp_pack_t
 * setting_not_ip_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_ip_address_destantion_udp_pack( \
        udp_pack_t pack, const char * const ip);

/**
 * @brief Function addition data in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] data Addition data in UDP package.
 * @param[in] size Size addition data for UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * const char * const message = "Hello, world";
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = add_data_udp_pack(pack, message, strlen(message));
 * if (ret == -1)
 *     gotov add_not_data;
 * add_not_data:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t add_data_udp_pack(udp_pack_t pack, void * data, uint16_t size);

/**
 * @brief Function overriding data in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] data New data in UDP package.
 * @param[in] size Size new data for UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * const char * const message = "Hello, world";
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_data_udp_pack(pack, message, strlen(message));
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_data_udp_pack(udp_pack_t pack, void * data,  \
        const uint16_t size);

/**
 * @brief Function addition byte in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] byte Addition byte in UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = add_byte_udp_pack(pack, 'n');
 * if (ret)
 *     goto add_not_byte;
 * ret = add_byte_udp_pack(pack, '\0');
 *     goto add_not_byte;
 * add_not_byte:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t add_byte_udp_pack(udp_pack_t pack, \
        const uint8_t byte);

/**
 * @brief Function read data from stdin in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_input_data_udp_pack(pack);
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_input_data_udp_pack(udp_pack_t pack);

/**
 * @brief Function read data from file in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] file_name File name for read in UDP pack.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_file_data_udp_pack(pack, "Hello.txt");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_file_data_udp_pack( \
        udp_pack_t pack, const char * const file_name);

/**
 * @brief Function for pick interface to send UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] interface Interface to send UDP pack.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_interface_udp_pack(pack, "lo");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_interface_udp_pack( \
        udp_pack_t pack, const char * const interface);

/**
 * @brief Function to send UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * send_udp_pack(pack);
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t send_udp_pack(udp_pack_t pack);

/**
 * @brief Function getting mac address for source.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_mac_address_source_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_mac_address_source_udp_pack(pack, "ff:ff:ff:ff:ff:ff");
 * if (ret == -1)
 *     goto setting_not_mac_address;
 * {
 *     char * mac_address =  NULL;
 *     mac_address = get_mac_address_source_udp_pack(pack);
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * setting_not_mac_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_mac_address_source_udp_pack(udp_pack_t pack);

/**
 * @brief Function setting mac address for destantion.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_mac_address_destantion_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_mac_address_destantion_udp_pack(pack, "ff:ff:ff:ff:ff:ff");
 * if (ret == -1)
 *     goto setting_not_mac_address;
 * {
 *     char * mac_address = NULL;
 *     mac_address = get_mac_address_destantion_udp_pack(udp_pack pack);
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * setting_not_mac_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_mac_address_destantion_udp_pack(udp_pack_t pack);

/**
 * @brief Function setting mac address for source.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] mac_address String is mac address source.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_mac_address_source_udp_pack(pack, "ff:ff:ff:ff:ff:ff");
 * if (ret == -1)
 *     goto setting_not_mac_address;
 * // other code whit udp_pack_t
 * setting_not_mac_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_mac_address_source_udp_pack(udp_pack_t pack, \
        const char * const mac_address);

/**
 * @brief Function setting mac address for destantion.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] mac_address String is mac address destantion.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_mac_address_destantion_udp_pack(pack, "ff:ff:ff:ff:ff:ff");
 * if (ret == -1)
 *     goto setting_not_mac_address;
 * // other code whit udp_pack_t
 * setting_not_mac_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_mac_address_destantion_udp_pack(udp_pack_t pack, \
        const char * const mac_address);

/**
 * @brief Function for getting interface.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_interface_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_interface_udp_pack(pack, "lo");
 * if (ret == -1)
 *     goto setting_not_interface;
 * {
 *     char * interface = NULL;
 *     interface = get_interface_udp_pack(pack);
 *     if (interface == NULL)
 *         getting_not_interface;
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_interface:
 * setting_not_interface:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_interface_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting port source.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_port_source_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_port_source_udp_pack(pack, "8003");
 * if (ret == -1)
 *     goto setting_not_port_source;
 * {
 *     char * port_source = NULL;
 *     port_source = get_port_source_udp_pack(pack);
 *     if (port_source == NULL)
 *         getting_not_port_source;
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_port_source:
 * setting_not_port_source:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_port_source_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting port destantion.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_port_destantion_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_port_destantion_udp_pack(pack, "8001");
 * if (ret == -1)
 *     goto setting_not_port_destantion;
 * {
 *     char * port_destantion = NULL;
 *     port_destantion = get_port_destantion_udp_pack(pack);
 *     if (port_destantion == NULL)
 *         getting_not_port_destantion;
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_port_destantion:
 * setting_not_port_destantion:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_port_destantion_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting ip address source.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_ip_address_source_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_ip_address_source_udp_pack(pack, "127.0.0.1");
 * if (ret == -1)
 *     goto setting_not_ip_address;
 * {
 *     char * ip_address = NULL;
 *     ip_address = get_ip_address_source_udp_pack(pack);
 *     if (ip_source == NULL)
 *         getting_not_ip_address;
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_ip_address:
 * setting_not_ip_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_ip_address_source_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting ip address destantion.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_ip_address_destantion_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return String or NULL pointer is fail.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = set_ip_address_source_udp_pack(pack, "127.0.0.1");
 * if (ret == -1)
 *     goto setting_not_ip_destantion;
 * {
 *     char * ip_address = NULL;
 *     ip_address = get_ip_destantion_udp_pack(pack);
 *     if (ip_address == NULL)
 *         getting_not_ip_address;
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_ip_address:
 * setting_not_ip_address:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_ip_address_destantion_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting size data.
 * @note You must call @ref init_udp_pack before this.
 * @note You can call @ref set_data_udp_pack before this.
 * @note You can call @ref add_data_udp_pack before this.
 * @note You can call @ref add_byte_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return Length data in UDP package.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * {
 *     uint16_t size_data = get_size_data_udp_pack(pack);
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * getting_not_size_data:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
uint16_t get_size_data_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting data.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return Data in UDP package.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * {
 *     void * data = get_data_udp_pack(pack);
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_data_udp_pack(udp_pack_t pack);

/**
 * @brief Function for getting data hex.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return Data hex in UDP package or NULL pointer on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * {
 *     char * data = get_data_hex_udp_pack(pack);
 *     if (data == NULL) {
 *         ret = -1;
 *         goto get_not_data_hex_udp_pack;
 *     }
 *     // other code whit udp_pack_t
 * }
 * // other code whit udp_pack_t
 * get_not_data_hex_udp_pack:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
char * get_data_hex_udp_pack(udp_pack_t pack);

/**
 * @brief Function print all info about udp pack.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * ret = print_udp_pack(pack);
 * if (ret)
 *     goto print_not_udp_pack;
 * // other code whit udp_pack_t
 * print_not_udp_pack:
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t print_udp_pack(udp_pack_t pack);

/**
 * @brief Function free UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * // other code whit using udp_pack_t
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
void destroy_udp_pack(udp_pack_t pack);

/** @} */
